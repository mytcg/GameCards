#include <conprint.h>

#include "AlbumViewScreen.h"
#include "../utils/Util.h"
#include "../utils/MAHeaders.h"
#include "ImageScreen.h"
#include "OptionsScreen.h"

AlbumViewScreen::AlbumViewScreen(Screen *previous, Feed *feed, String category, int albumType, Map<String, Card*> map) : mHttp(this),
filename(category+ALBUMEND), category(category), previous(previous), feed(feed), cardExists(cards.end()), albumType(albumType) {
	busy = true;
	emp = true;
	feedLayouts = NULL;

	next = NULL;
	error_msg = "";
	id = "";
	description = "";
	quantity = "";
	thumburl = "";
	fronturl = "";
	backurl = "";
	rate = "";
	value = "";
	updated = "";
	note = "";
	searchString = "";
	#if defined(MA_PROF_SUPPORT_STYLUS)
		mainLayout = createMainLayout(back, options, "", true);
	#else
		mainLayout = createMainLayout(back, options, select, true);
	#endif
	listBox = (KineticListBox*) mainLayout->getChildren()[0]->getChildren()[2];
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];

	mImageCache = new ImageCache();
	switch (albumType) {
		case AT_SEARCH:
			notice->setCaption("Searching...");
			cards = map;
			drawList();
			notice->setCaption("");
			busy = false;
			break;
		default:
			notice->setCaption(checking_cards);
			loadFile();
			//work out how long the url will be, the 15 is for the & and = symbals, as well as hard coded parameters
			int urlLength = CARDS.length() + category.length() + 24 + intlen(getMaxImageHeight()) + intlen(scrWidth) + feed->getSeconds().length();
			char *url = new char[urlLength];
			memset(url,'\0',urlLength);
			sprintf(url, "%s%s&seconds=%s&height=%d&width=%d", CARDS.c_str(), category.c_str(), feed->getSeconds().c_str(), getMaxImageHeight(), scrWidth);
			if(mHttp.isOpen()){
				mHttp.close();
			}
			mHttp = HttpConnection(this);
			int res = mHttp.create(url, HTTP_GET);
			if(res < 0) {
				busy = false;
				hasConnection = false;
				notice->setCaption("");
			} else {
				hasConnection = true;
				mHttp.setRequestHeader(auth_user, feed->getUsername().c_str());
				mHttp.setRequestHeader(auth_pw, feed->getEncrypt().c_str());
				mHttp.finish();
			}
			delete [] url;
			break;
	}
	this->setMain(mainLayout);
	moved=0;
	origAlbum = this;
}

void AlbumViewScreen::refresh() {
	int urlLength = 0;
	char *url = NULL;
	switch (albumType) {
		case AT_SEARCH:
			notice->setCaption("Searching...");
			urlLength = SEARCH.length() + searchString.length() + strlen(seconds) + feed->getSeconds().length()
				+ strlen(height) + intlen(getMaxImageHeight()) + strlen(width) + intlen(scrWidth) + 6;
			url = new char[urlLength];
			memset(url,'\0',urlLength);
			sprintf(url, "%s%s&%s=%s&%s=%d&%s=%d", SEARCH.c_str(), searchString.c_str(), seconds,
				feed->getSeconds().c_str(), height, getMaxImageHeight(), width, scrWidth);
			break;
		default:
			tmp.clear();
			notice->setCaption(checking_cards);
			//work out how long the url will be, the 15 is for the & and = symbals, as well as hard coded parameters
			urlLength = CARDS.length() + category.length() + 24 + intlen(getMaxImageHeight()) + intlen(scrWidth) + feed->getSeconds().length();
			url = new char[urlLength];
			memset(url,'\0',urlLength);
			sprintf(url, "%s%s&seconds=%s&height=%d&width=%d", CARDS.c_str(), category.c_str(), feed->getSeconds().c_str(), getMaxImageHeight(), scrWidth);
			break;
	}
	mHttp = HttpConnection(this);
	int res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		hasConnection = false;
		notice->setCaption("");
	} else {
		hasConnection = true;
		mHttp.setRequestHeader(auth_user, feed->getUsername().c_str());
		mHttp.setRequestHeader(auth_pw, feed->getEncrypt().c_str());
		mHttp.finish();
	}
	delete [] url;
	show();
}

void AlbumViewScreen::loadFile() {
	loadImages(getData(filename.c_str()));
}

void AlbumViewScreen::loadImages(const char *text) {
	String all = text;
	int indexof = 0;
	int indentindexof = 0;
	String tmp;
	String id, name;
	while ((indexof = all.find(newline)) > -1) {
		tmp = all.substr(0,indexof++);
		Card *newCard = new Card();
		newCard->setAll(tmp.c_str());
		cards.insert(newCard->getId(), newCard);
		all = all.substr(indexof);
	}
	drawList();
}

#if defined(MA_PROF_SUPPORT_STYLUS)

void AlbumViewScreen::pointerPressEvent(MAPoint2d point) {
    locateItem(point);
}

void AlbumViewScreen::pointerMoveEvent(MAPoint2d point) {
    locateItem(point);
    moved++;
}

void AlbumViewScreen::pointerReleaseEvent(MAPoint2d point) {
	if (moved <= 8) {
		if (right) {
			keyPressEvent(MAK_SOFTRIGHT);
		} else if (left) {
			keyPressEvent(MAK_SOFTLEFT);
		} else if (list) {
			keyPressEvent(MAK_FIRE);
		}
	}
	moved=0;
}

void AlbumViewScreen::locateItem(MAPoint2d point) {
	if (feed->setTouch(truesz)) {
		saveData(FEED, feed->getAll().c_str());
	}
	list = false;
	left = false;
	right = false;

	Point p;
	p.set(point.x, point.y);
	for(int i = 0; i < (this->getMain()->getChildren()[0]->getChildren()[2]->getChildren()).size(); i++)
	{
		if(this->getMain()->getChildren()[0]->getChildren()[2]->getChildren()[i]->contains(p))
		{
			list = true;
		}
	}
	for(int i = 0; i < (this->getMain()->getChildren()[1]->getChildren()).size(); i++)
	{
		if(this->getMain()->getChildren()[1]->getChildren()[i]->contains(p))
		{
			if (i == 0) {
				left = true;
				moved=0;
			} else if (i == 2) {
				right = true;
				moved=0;
			}
			return;
		}
	}
}

#endif

void AlbumViewScreen::clearListBox() {
	for (int i = 0; i < listBox->getChildren().size(); i++) {
		tempWidgets.add(listBox->getChildren()[i]);
	}
	listBox->clear();
	listBox->getChildren().clear();

	for (int j = 0; j < tempWidgets.size(); j++) {
		delete tempWidgets[j];
		tempWidgets[j] = NULL;
	}
	tempWidgets.clear();
}

void AlbumViewScreen::drawList() {
	Layout *feedlayout;
	//listBox->clear();
	clearListBox();
	index.clear();
	for(StringCardMap::Iterator itr = cards.begin(); itr != cards.end(); itr++) {

		index.add(itr->second->getId());
		cardText = (itr->second->getUpdated()?updated_symbol:"")+itr->second->getText();
		cardText += "\nQuantity: ";
		cardText += itr->second->getQuantity();

		feedlayout = new Layout(0, 0, listBox->getWidth(), 74, listBox, 3, 1);
		feedlayout->setSkin(gSkinAlbum);
		feedlayout->setDrawBackground(true);
		feedlayout->addWidgetListener(this);

		if (strcmp(itr->second->getQuantity().c_str(), "0") != 0) {
			//if the user has one or more of the card, the image must be downloaded
			tempImage = new MobImage(0, 0, 56, 64, feedlayout, false, false, RES_LOADINGTHUMB);
			tempImage->setHasNote(itr->second->getNote().length()>0);
			retrieveThumb(tempImage, itr->second, mImageCache);
		}
		else {
			//we use the blank image for cards they dont have yet
			tempImage = new MobImage(0, 0, 56, 64, feedlayout, false, false, RES_MISSINGTHUMB);
		}

		label = new Label(0,0, scrWidth-86, 74, feedlayout, cardText, 0, gFontBlack);
		label->setVerticalAlignment(Label::VA_CENTER);
		label->setAutoSizeY();
		label->setAutoSizeX(true);
		label->setMultiLine(true);
	}

	if (cards.size() >= 1) {
		emp = false;
		listBox->setSelectedIndex(0);
	} else {
		emp = true;
		listBox->add(createSubLabel(empty));
		listBox->setSelectedIndex(0);
	}
}

AlbumViewScreen::~AlbumViewScreen() {
	delete mainLayout;
	if(next!=NULL){
		delete next;
	}
	delete mImageCache;
	if(feedLayouts!=NULL){
		delete [] feedLayouts;
	}
	if (albumType != AT_SEARCH) {
		saveData(filename.c_str(), getAll().c_str());
	}
	clearCardMap();
	tmp.clear();
	parentTag="";
	cardText="";
	id="";
	description="";
	quantity="";
	thumburl="";
	fronturl="";
	backurl="";
	filename="";
	error_msg="";
	rate="";
	value="";
	updated="";
	note="";
	category="";
}

void AlbumViewScreen::selectionChanged(Widget *widget, bool selected) {
	if(selected) {
		((Label *)widget->getChildren()[1])->setFont(gFontBlue);
	} else {
		((Label *)widget->getChildren()[1])->setFont(gFontBlack);
	}
}

void AlbumViewScreen::show() {
	listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(true);
	Screen::show();
}

void AlbumViewScreen::hide() {
    listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(false);
	Screen::hide();
}

void AlbumViewScreen::keyPressEvent(int keyCode) {
	int selected = listBox->getSelectedIndex();
	switch(keyCode) {
		case MAK_UP:
			listBox->selectPreviousItem();
			break;
		case MAK_DOWN:
			listBox->selectNextItem();
			break;
		case MAK_SOFTLEFT:
			if (albumType != AT_SEARCH) {
				saveData(filename.c_str(), getAll().c_str());
			}
			previous->show();
			break;
		case MAK_FIRE:
			if (!emp && !busy && strcmp(cards.find(index[selected])->second->getQuantity().c_str(), "0") != 0) {
				if (next != NULL) {
					delete next;
				}
				if (albumType == AT_NEW_CARDS) {
					next = new ImageScreen(this, RES_LOADING, feed, false, cards.find(index[selected])->second, ImageScreen::ST_NEW_CARD);
				}
				else {
					next = new ImageScreen(this, RES_LOADING, feed, false, cards.find(index[selected])->second);
				}
				next->show();
			}
			break;
		case MAK_SOFTRIGHT:
			if (!emp && !hasConnection) {
				notice->setCaption(no_connect);
			}
			else if (!emp && !busy && strcmp(cards.find(index[selected])->second->getQuantity().c_str(), "0") != 0) {
				if (next != NULL) {
					delete next;
				}
				if (albumType == AT_NEW_CARDS) {
					next = new OptionsScreen(feed, OptionsScreen::ST_NEW_CARD,
							this, cards.find(index[selected])->second);
				}
				else {
					next = new OptionsScreen(feed, OptionsScreen::ST_CARD_OPTIONS,
							this, cards.find(index[selected])->second);
				}
				next->show();
			}
			break;
	}
}

void AlbumViewScreen::httpFinished(MAUtil::HttpConnection* http, int result) {
	error_msg = "";
	if (result == 200) {
		xmlConn = XmlConnection::XmlConnection();
		xmlConn.parse(http, this, this);
	} else {
		mHttp.close();
		notice->setCaption("");
		busy = false;
	}
}

void AlbumViewScreen::connReadFinished(Connection* conn, int result) {}

void AlbumViewScreen::xcConnError(int code) {
}

void AlbumViewScreen::mtxEncoding(const char* ) {
}

void AlbumViewScreen::mtxTagStart(const char* name, int len) {
	parentTag = name;
}

void AlbumViewScreen::mtxTagAttr(const char* attrName, const char* attrValue) {
	if(!strcmp(parentTag.c_str(), xml_stat)) {
		if(!strcmp(attrName, xml_desc)) {
			statDesc += attrValue;
		} else if(!strcmp(attrName, xml_ival)) {
			statIVal += attrValue;
		}
	}
}

void AlbumViewScreen::mtxTagData(const char* data, int len) {
	if(!strcmp(parentTag.c_str(), xml_cardid)) {
		id += data;
	} else if(!strcmp(parentTag.c_str(), xml_carddescription)) {
		description += data;
	} else if(!strcmp(parentTag.c_str(), xml_cardquantity)) {
		quantity += data;
	} else if(!strcmp(parentTag.c_str(), xml_thumburl)) {
		thumburl += data;
	} else if(!strcmp(parentTag.c_str(), xml_fronturl)) {
		fronturl += data;
	} else if(!strcmp(parentTag.c_str(), xml_backurl)) {
		backurl += data;
	} else if(!strcmp(parentTag.c_str(), xml_rate)) {
		rate += data;
	} else if(!strcmp(parentTag.c_str(), xml_value)) {
		value += data;
	} else if(!strcmp(parentTag.c_str(), xml_error)) {
		error_msg += data;
	} else if(!strcmp(parentTag.c_str(), xml_updated)) {
		updated += data;
	} else if(!strcmp(parentTag.c_str(), xml_stat)) {
		statDisplay += data;
	} else if(!strcmp(parentTag.c_str(), xml_note)) {
		note += data;
	}
}

void AlbumViewScreen::mtxTagEnd(const char* name, int len) {
	if(!strcmp(name, xml_card)) {
		notice->setCaption("");
		Card *newCard = new Card();
		newCard->setAll((quantity+delim+description+delim+thumburl+delim+fronturl+delim+backurl+delim+id+delim+rate+delim+value+delim+note+delim).c_str());
		newCard->setStats(stats);
		cardExists = cards.find(newCard->getId());
		if (cardExists != cards.end()) {
			newCard->setThumb(cardExists->second->getThumb().c_str());
			newCard->setBack(cardExists->second->getBack().c_str());
			newCard->setFront(cardExists->second->getFront().c_str());
		}
		newCard->setUpdated(updated == "1");
		tmp.insert(newCard->getId(),newCard);
		id = "";
		description = "";
		quantity = "";
		thumburl = "";
		fronturl = "";
		backurl = "";
		rate = "";
		value = "";
		updated = "";
		note = "";
		stats.clear();
	} else if(!strcmp(name, xml_stat)) {
		stat = new Stat();
		stat->setDesc(statDesc.c_str());
		stat->setDisplay(statDisplay.c_str());
		stat->setIVal(statIVal.c_str());

		stats.add(stat);

		statDesc = "";
		statDisplay = "";
		statIVal = "";
	} else if(!strcmp(name, xml_error)) {
		notice->setCaption(error_msg.c_str());
	} else if (!strcmp(name, xml_carddone)) {
		notice->setCaption("");
		clearCardMap();
		cards = tmp;
		drawList();
		busy = false;
		saveData(filename.c_str(), getAll().c_str());
	} else {
		notice->setCaption("");
	}
}

String AlbumViewScreen::getAll() {
	String all;
	for(StringCardMap::Iterator itr = cards.begin(); itr != cards.end(); itr++) {
		all += itr->second->getAll() + "#";
	}
	return all;
}

void AlbumViewScreen::clearCardMap() {
	for (StringCardMap::Iterator iter = cards.begin(); iter != cards.end(); iter++) {
		if (iter->second != NULL) {
			delete iter->second;
			iter->second = NULL;
		}
	}
	cards.clear();
}

void AlbumViewScreen::mtxParseError() {
}

void AlbumViewScreen::mtxEmptyTagEnd() {
}

void AlbumViewScreen::mtxTagStartEnd() {
}

int AlbumViewScreen::getCount() {
	return size;
}

void AlbumViewScreen::setSearchString(String ss) {
	searchString = ss;
}
