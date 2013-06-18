#include <conprint.h>
#include <madmath.h>
#include <MAUI/Image.h>

#include "NewDeckScreen.h"
#include "EditDeckScreen.h"
#include "DeckListScreen.h"
#include "OptionsScreen.h"
#include "../utils/Util.h"
#include "../UI/Button.h"

DeckListScreen::DeckListScreen(MainScreen *previous, Feed *feed, int screenType, int screenSubType, String categoryId)
		:mHttp(this), screenType(screenType), screenSubType(screenSubType), categoryId(categoryId) {
	lprintfln("DeckListScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	this->previous = previous;
	this->feed = feed;
	currentSelectedKey = NULL;
	currentKeyPosition = -1;
	mainLayout = Util::createMainLayout("", "Back", true);
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];
	kinListBox = (KineticListBox*)mainLayout->getChildren()[0]->getChildren()[2];

	this->setMain(mainLayout);

	deckId = "";
	description = "";
	active="";
	type="";
	moved = 0;

	album = NULL;
	deck = NULL;
	next = NULL;

	selecting = true;

	notice->setCaption("Loading decks...");

	int urlLength = 0;
	char *url = NULL;

	switch (screenType) {
		case ST_EDIT:
			switch (screenSubType) {
				case SST_NORMAL:
					//work out how long the url will be, the 2 is for the & and = symbals, as well as hard coded vars
					urlLength = strlen("?getallnormaldecks=1") + URLSIZE;
					url = new char[urlLength+1];
					memset(url,'\0',urlLength+1);
					sprintf(url, "%s?getallnormaldecks=1", URL);
					lprintfln("%s", url);
					break;
				case SST_COMPETITION:
					//work out how long the url will be, the 2 is for the & and = symbals, as well as hard coded vars
					urlLength = strlen("?getallcompdecks=1") + URLSIZE;
					url = new char[urlLength+1];
					memset(url,'\0',urlLength+1);
					sprintf(url, "%s?getallcompdecks=1", URL);
					lprintfln("%s", url);
					break;
			}
			break;
		case ST_SELECT:
			//work out how long the url will be, the 2 is for the & and = symbals, as well as hard coded vars
			urlLength = strlen("?getcategorydecks=1&category_id=") + categoryId.length() + URLSIZE;
			url = new char[urlLength+1];
			memset(url,'\0',urlLength+1);
			sprintf(url, "%s?getcategorydecks=1&category_id=%s", URL, categoryId.c_str());
			lprintfln("%s", url);
			break;
	}

	if(mHttp.isOpen()){
		mHttp.close();
	}
	mHttp = HttpConnection(this);
	int res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		notice->setCaption("Connection error.");
	} else {
		mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
		mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
		feed->addHttp();
		mHttp.finish();

	}

	urlLength = 0;
	if (url != NULL) {
		delete [] url;
		url = NULL;
	}
}

DeckListScreen::~DeckListScreen() {
	lprintfln("~DeckListScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	delete mainLayout;
	mainLayout = NULL;
	currentSelectedKey = NULL;
	parentTag= "";
	description = "";
	active="";
	deckId = "";
	categoryId = "";
	type="";

	if (next != NULL) {
		delete next;
		feed->remHttp();
		next = NULL;
	}
	for (int i = 0; i < albums.size(); i++) {
		delete albums[i];
		albums[i] = NULL;
	}
	albums.clear();
	for (int i = 0; i < decks.size(); i++) {
		delete decks[i];
		decks[i] = NULL;
	}
	decks.clear();
}

void DeckListScreen::refresh() {
	show();
	clearListBox();
	clearAlbums();

	selecting = true;

	notice->setCaption("Loading decks...");

	int urlLength = 0;
	char *url = NULL;

	switch (screenType) {
		case ST_EDIT:
			switch (screenSubType) {
				case SST_NORMAL:
					//work out how long the url will be, the 2 is for the & and = symbals, as well as hard coded vars
					urlLength = strlen("?getallnormaldecks=1") + URLSIZE;
					url = new char[urlLength+1];
					memset(url,'\0',urlLength+1);
					sprintf(url, "%s?getallnormaldecks=1", URL);
					break;
				case SST_COMPETITION:
					//work out how long the url will be, the 2 is for the & and = symbals, as well as hard coded vars
					urlLength = strlen("?getallcompdecks=1") + URLSIZE;
					url = new char[urlLength+1];
					memset(url,'\0',urlLength+1);
					sprintf(url, "%s?getallcompdecks=1", URL);
					break;
			}
			break;
		case ST_SELECT:
			//work out how long the url will be, the 2 is for the & and = symbals, as well as hard coded vars
			urlLength = strlen("?getcategorydecks=1&category_id=") + categoryId.length() + URLSIZE;
			url = new char[urlLength+1];
			memset(url,'\0',urlLength+1);
			sprintf(url, "%s?getcategorydecks=1&category_id=%s", URL, categoryId.c_str());
			break;
	}
	lprintfln("%s", url);

	if(mHttp.isOpen()){
		mHttp.close();
	}
	mHttp = HttpConnection(this);
	int res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		notice->setCaption("Connection error.");
	} else {
		mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
		mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
		feed->addHttp();
		mHttp.finish();
	}

	urlLength = 0;
	if (url != NULL) {
		delete [] url;
		url = NULL;
	}
}

void DeckListScreen::clearAlbums() {
	for (int i = 0; i < albums.size(); i++) {
		delete albums[i];
		albums = NULL;
	}
	for (int i = 0; i < decks.size(); i++) {
		delete decks[i];
		decks[i] = NULL;
	}
	albums.clear();
	decks.clear();
}

void DeckListScreen::clearListBox() {
	Vector<Widget*> tempWidgets;
	for (int i = 0; i < kinListBox->getChildren().size(); i++) {
		tempWidgets.add(kinListBox->getChildren()[i]);
	}
	kinListBox->clear();
	kinListBox->getChildren().clear();

	for (int j = 0; j < tempWidgets.size(); j++) {
		delete tempWidgets[j];
		tempWidgets[j] = NULL;
	}
	tempWidgets.clear();
}

void DeckListScreen::drawList() {
	if (screenType == ST_EDIT && screenSubType == SST_NORMAL) {
		label = Util::createSubLabel("New Deck");
		label->addWidgetListener(this);
		kinListBox->add(label);
	}
	for(int i = 0; i < albums.size(); i++) {
		label = Util::createSubLabel(albums[i]->getDescription());
		label->addWidgetListener(this);
		kinListBox->add(label);
	}

	if (currentSelectedKey!=NULL) {
		currentSelectedKey->setSelected(false);
		currentSelectedKey = NULL;
		currentKeyPosition = -1;
	}

	if ((albums.size() > 0 && screenSubType == SST_COMPETITION) || screenSubType == SST_NORMAL) {
		kinListBox->setSelectedIndex(0);
	}

	selecting = false;
}

void DeckListScreen::pointerPressEvent(MAPoint2d point)
{
    locateItem(point);
}

void DeckListScreen::pointerMoveEvent(MAPoint2d point)
{
	moved++;
    locateItem(point);
}

void DeckListScreen::pointerReleaseEvent(MAPoint2d point)
{
	if (moved <= 4) {
		if (right) {
			keyPressEvent(MAK_SOFTRIGHT);
		} else if (left) {
			keyPressEvent(MAK_SOFTLEFT);
		} else if (list) {
			keyPressEvent(MAK_FIRE);
		}
	}
	moved= 0;
}

void DeckListScreen::locateItem(MAPoint2d point)
{
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
			} else if (i == 2) {
				right = true;
			}
			return;
		}
	}
}

void DeckListScreen::keyPressEvent(int keyCode) {
	int ind = kinListBox->getSelectedIndex();
	int max = kinListBox->getChildren().size();
	Widget *currentSoftKeys = mainLayout->getChildren()[mainLayout->getChildren().size() - 1];
	switch(keyCode) {
		case MAK_SOFTRIGHT:
		case MAK_BACK:
			previous->show();
			break;
		case MAK_FIRE:
			if(currentSoftKeys->getChildren()[0]->isSelected()){
				keyPressEvent(MAK_SOFTLEFT);
				break;
			}else if(currentSoftKeys->getChildren()[2]->isSelected()){
				keyPressEvent(MAK_SOFTRIGHT);
				break;
			}
		case MAK_SOFTLEFT:
			if (!selecting) {
				switch (screenType) {
					case ST_EDIT:
						if (screenSubType == SST_NORMAL && kinListBox->getSelectedIndex() == 0) {
							if (next != NULL) {
								delete next;
								feed->remHttp();
								next = NULL;
							}
							next = new NewDeckScreen(this, feed);
							next->show();
						}
						else {
							int index = kinListBox->getSelectedIndex();
							if (screenSubType == SST_NORMAL) {
								index--;
							}

							if (next != NULL) {
								delete next;
								feed->remHttp();
								 next = NULL;
							}
							next = new EditDeckScreen(this, feed, albums[index]->getId(),
									decks[index]->getType(),decks[index]->getActive());
							next->show();

							index = 0;
						}
						break;
					case ST_SELECT:
						next = new OptionsScreen(feed, OptionsScreen::ST_NEW_GAME_OPTIONS, this, NULL,
							categoryId, albums[kinListBox->getSelectedIndex()]->getId());
						next->show();
						break;
				}
			}
			break;
		case MAK_DOWN:
			if (ind+1 < max && !selecting) {
				kinListBox->setSelectedIndex(ind+1);
			} else {
				if (!selecting && max > 0) {
					kinListBox->getChildren()[ind]->setSelected(false);
				}
				for(int i = 0; i < currentSoftKeys->getChildren().size();i++){
					if(((Button *)currentSoftKeys->getChildren()[i])->isSelectable()){
						currentKeyPosition=i;
						currentSelectedKey= currentSoftKeys->getChildren()[i];
						currentSelectedKey->setSelected(true);
						break;
					}
				}
			}
			break;
		case MAK_UP:
			if(currentSelectedKey!=NULL){
				currentSelectedKey->setSelected(false);
				currentSelectedKey = NULL;
				currentKeyPosition = -1;
				if (!selecting && max > 0) {
					kinListBox->getChildren()[kinListBox->getChildren().size()-1]->setSelected(true);
				}
			}
			else if (!selecting && max > 0) {
				if (ind == 0) {
					kinListBox->setSelectedIndex(max-1);
				} else {
					kinListBox->selectPreviousItem();
				}
			}
			break;
		case MAK_LEFT:
			if(currentSelectedKey!=NULL){
				if(currentKeyPosition > 0){
					currentKeyPosition = currentKeyPosition - 1;
					for(int i = currentKeyPosition; i >= 0;i--){
						if(((Button *)currentSoftKeys->getChildren()[i])->isSelectable()){
							currentSelectedKey->setSelected(false);
							currentKeyPosition=i;
							currentSelectedKey= currentSoftKeys->getChildren()[i];
							currentSelectedKey->setSelected(true);
							break;
						}
					}
				}
			}
			break;
		case MAK_RIGHT:
			if(currentSelectedKey!=NULL){
				if(currentKeyPosition+1 < currentSelectedKey->getParent()->getChildren().size()){
					currentKeyPosition = currentKeyPosition + 1;
					for(int i = currentKeyPosition; i < currentSoftKeys->getChildren().size();i++){
						if(((Button *)currentSoftKeys->getChildren()[i])->isSelectable()){
							currentSelectedKey->setSelected(false);
							currentKeyPosition=i;
							currentSelectedKey= currentSoftKeys->getChildren()[i];
							currentSelectedKey->setSelected(true);
							break;
						}
					}
				}
			}
			break;
	}
}

void DeckListScreen::selectionChanged(Widget *widget, bool selected) {
	if(selected) {
		((Label *)widget)->setFont(Util::getDefaultSelected());
	} else {
		((Label *)widget)->setFont(Util::getDefaultFont());
	}
}

void DeckListScreen::httpFinished(MAUtil::HttpConnection* http, int result) {
	if (result == 200) {
		xmlConn = XmlConnection::XmlConnection();
		xmlConn.parse(http, this, this);
	} else {
		mHttp.close();
		feed->remHttp();
		notice->setCaption("Unable to connect, try again later...");
	}
}

void DeckListScreen::connReadFinished(Connection* conn, int result) {}

void DeckListScreen::xcConnError(int code) {
	feed->remHttp();
}

void DeckListScreen::mtxEncoding(const char* ) {
}

void DeckListScreen::mtxTagStart(const char* name, int len) {
	parentTag = name;
}

void DeckListScreen::mtxTagAttr(const char* attrName, const char* attrValue) {
}

void DeckListScreen::mtxTagData(const char* data, int len) {
	if(!strcmp(parentTag.c_str(), "deck_id")) {
		deckId += data;
	} else if(!strcmp(parentTag.c_str(), "desc")) {
		description += data;
	} else if(!strcmp(parentTag.c_str(), "active")) {
		active += data;
	} else if(!strcmp(parentTag.c_str(), "type")) {
		type += data;
	}
}

void DeckListScreen::mtxTagEnd(const char* name, int len) {
	if (!strcmp(name, "deck")) {
		album = new Album(deckId, description);
		albums.add(album);
		deck = new Deck(deckId,description,type,active);
		decks.add(deck);
		deckId = "";
		description = "";
		active="";
		type="";
	}
	else if (!strcmp(name, "decks")) {
		if (albums.size() <= 1 && screenType == ST_SELECT) {
			if (albums.size() == 0) {
				next = new OptionsScreen(feed, OptionsScreen::ST_NEW_GAME_OPTIONS, previous, NULL, categoryId, "-1");
				next->show();
			}
			else {
				next = new OptionsScreen(feed, OptionsScreen::ST_NEW_GAME_OPTIONS, previous, NULL, categoryId, albums[0]->getId());
				next->show();
			}
		}
		else {
			drawList();
		}
		notice->setCaption("");
	}
}

void DeckListScreen::mtxParseError(int) {
}

void DeckListScreen::mtxEmptyTagEnd() {
}

void DeckListScreen::mtxTagStartEnd() {
}
