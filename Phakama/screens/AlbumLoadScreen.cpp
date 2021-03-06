#include <conprint.h>

#include "AlbumLoadScreen.h"
#include "AlbumViewScreen.h"
#include "../utils/Util.h"
#include "../utils/Albums.h"
#include "OptionsScreen.h"
#include "../utils/Album.h"

void AlbumLoadScreen::refresh() {
	show();
	path.clear();
	if(mHttp.isOpen()){
		mHttp.close();
	}

	int res = -1;
	char *url = NULL;
	int urlLength = 0;

	mHttp = HttpConnection(this);
	//delete album;
	if (album == NULL) {
		album = new Albums();
	} else {
		album->clearAll();
	}
	String alb = this->feed->getAlbum()->getAll();
	album->setAll(alb.c_str());
	alb = "";
	drawList();
	notice->setCaption("Checking for new albums...");
	urlLength = 52+URLSIZE + feed->getSeconds().length();
	url = new char[urlLength+1];
	memset(url,'\0',urlLength+1);
	sprintf(url, "%s?usercategories=1&seconds=%s", URL, feed->getSeconds().c_str());
	res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		hasConnection = false;
		notice->setCaption("");
	} else {
		hasConnection = true;
		mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
		mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
		feed->addHttp();
		mHttp.finish();
	}
	this->setMain(mainLayout);

	if (url != NULL) {
		delete url;
	}
}

AlbumLoadScreen::AlbumLoadScreen(Screen *previous, Feed *feed, int screenType, Albums *a, Card *card, String categoryId) : mHttp(this),
		previous(previous), feed(feed), screenType(screenType), card(card), categoryId(categoryId) {
	lprintfln("AlbumLoadScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	size = 0;
	moved = 0;
	int res = -1;
	char *url = NULL;
	int urlLength = 0;

	hasCards = "";
	temp = "";
	temp1 = "";
	deckId = "";
	updated = "0";

	next = NULL;
	mainLayout = Util::createMainLayout("", "Back", true);

	listBox = (KineticListBox*) mainLayout->getChildren()[0]->getChildren()[2];
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];

	album = new Albums();

	switch(screenType) {
		case ST_ALBUMS:
			notice->setCaption("Checking for new albums...");
			album->setAll(this->feed->getAlbum()->getAll().c_str());
			urlLength = 60 + URLSIZE + feed->getSeconds().length();
			url = new char[urlLength+1];
			memset(url,'\0',urlLength+1);
			sprintf(url, "%s?usercategories=1&seconds=%s", URL, feed->getSeconds().c_str());
			res = mHttp.create(url, HTTP_GET);
			break;
	}
	if(res < 0) {
		hasConnection = false;
		notice->setCaption("");
	} else {
		hasConnection = true;
		mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
		mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
		feed->addHttp();
		mHttp.finish();

	}
	drawList();

	this->setMain(mainLayout);

	if (url != NULL) {
		delete url;
	}
}

AlbumLoadScreen::~AlbumLoadScreen() {
	lprintfln("~AlbumLoadScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	delete mainLayout;
	if(next!=NULL){
		delete next;
		feed->remHttp();
	}
	parentTag="";
	temp="";
	temp1="";
	error_msg="";
	hasCards="";
	updated="";

	if (screenType == ST_ALBUMS) {
		if (album != NULL) {
			delete album;
			album = NULL;
		}
	}
}

void AlbumLoadScreen::pointerPressEvent(MAPoint2d point)
{
    locateItem(point);
}

void AlbumLoadScreen::pointerMoveEvent(MAPoint2d point)
{
	moved++;
    locateItem(point);
}

void AlbumLoadScreen::pointerReleaseEvent(MAPoint2d point)
{
	if (moved <= 8) {
		if (right) {
			keyPressEvent(MAK_SOFTRIGHT);
		} else if (left) {
			keyPressEvent(MAK_SOFTLEFT);
		} else if (list) {
			keyPressEvent(MAK_FIRE);
		} else if (mid) {
			keyPressEvent(MAK_FIRE);
		}
	}
	moved=0;
}

void AlbumLoadScreen::locateItem(MAPoint2d point)
{
	list = false;
	left = false;
	right = false;
	mid = false;

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
				moved=0;
				left = true;
			} else if (i == 1) {
				moved=0;
				mid = true;
			} else if (i == 2) {
				moved=0;
				right = true;
			}
			return;
		}
	}
}

void AlbumLoadScreen::drawList() {
	empt = false;
	clearListBox();
	listBox->clear();
	int ind = listBox->getSelectedIndex();
	Vector<String> display = album->getNames();
	String albumname = "";
	size = 0;
	for(Vector<String>::iterator itr = display.begin(); itr != display.end(); itr++) {
		albumname = itr->c_str();
		lprintfln("wawawa %s",albumname.c_str());
		if (strcmp(albumname.c_str(), "New Cards")) {
			label = Util::createSubLabel(albumname);
			label->setPaddingBottom(5);
			label->addWidgetListener(this);
			listBox->add(label);

			size++;
			lprintfln("size %d", size);
		}
		albumname = "";
	}
	albumname="";
	display.clear();

	if (album->size() >= 1) {
		if (ind < album->size()) {
			listBox->setSelectedIndex(ind);
		} else {
			listBox->setSelectedIndex(0);
		}
	} else {
		empt = true;
		label = Util::createSubLabel("Empty");
		label->addWidgetListener(this);
		listBox->add(label);
		size++;
	}
}

void AlbumLoadScreen::clearListBox() {
	Vector<Widget*> tempWidgets;
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

void AlbumLoadScreen::selectionChanged(Widget *widget, bool selected) {
	if(selected) {
		((Label *)widget)->setFont(Util::getDefaultSelected());
	} else {
		((Label *)widget)->setFont(Util::getDefaultFont());
	}
}

void AlbumLoadScreen::show() {
	listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(true);
	Screen::show();
}

void AlbumLoadScreen::hide() {
    listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(false);
	Screen::hide();
}

void AlbumLoadScreen::keyPressEvent(int keyCode) {
	int total = listBox->getChildren().size();
	int select = listBox->getSelectedIndex();
	switch(keyCode) {
		case MAK_UP:
			if (select == 0) {
				listBox->setSelectedIndex(total-1);
			} else {
				listBox->selectPreviousItem();
			}
			break;
		case MAK_DOWN:
			if (select == total-1) {
				listBox->setSelectedIndex(0);
			} else {
				listBox->selectNextItem();
			}
			break;
		case MAK_BACK:
		case MAK_SOFTRIGHT:
			switch(screenType) {
				default:
					if (path.size() > 0) {
						path.remove(path.size()-1);
						loadCategory();
					}
					else {
						previous->show();
					}
					break;
			}
			break;
		case MAK_SOFTLEFT:
		case MAK_FIRE:
			if (!empt) {
				Album* val = (album->getAlbum(((Label *)listBox->getChildren()[listBox->getSelectedIndex()])->getCaption()));
				if (next != NULL) {
					delete next;
					feed->remHttp();
					next = NULL;
				}
				switch (screenType) {
					case ST_ALBUMS:
						if (val->getHasCards()) {
							if (strcmp(val->getId().c_str(), "-3") == 0) {
								next = new AlbumViewScreen(this, feed, val->getId(), AlbumViewScreen::AT_NEW_CARDS);
								next->show();
							}
							else {
								next = new AlbumViewScreen(this, feed, val->getId(), AlbumViewScreen::AT_NORMAL);
								next->show();
							}
						}
						else {
							//if a category has no cards, it means it has sub categories.
							//it is added to the path so we can back track
							path.add(val->getId());
							//then it must be loaded
							loadCategory();
						}
						break;
				}
				//delete val;
			}
			break;
	}
}

void AlbumLoadScreen::loadCategory() {
	Util::updateSoftKeyLayout("", "Back", "", mainLayout);
	//the list needs to be cleared
	album->clearAll();
	clearListBox();
	listBox->clear();
	//then if the category has been loaded before, we need to load from the file
	notice->setCaption("Checking for new albums...");
	if (path.size() == 0) {
		album->setAll(this->feed->getAlbum()->getAll().c_str());
	}
	else {
		char *file = new char[path[path.size()-1].length() + 6];
		memset(file, 0, path[path.size()-1].length() + 6);
		sprintf(file, "a%s.sav", path[path.size()-1].c_str());
		String filecards = "";
		Util::getData(file, filecards);
		album->setAll(filecards.c_str());
		filecards = "";
		delete file;
	}
	drawList();
	//then request up to date info, if there is a connection available
	if (hasConnection) {
		int res;
		int urlLength;
		char *url = NULL;
		if(mHttp.isOpen()){
			mHttp.close();
		}
		mHttp = HttpConnection(this);
		if (path.size() == 0) {
			//if path is empty, the list is at the top level
			//work out how long the url will be, the 2 is for the & and = symbols
			urlLength = 60 + URLSIZE + feed->getSeconds().length();
			url = new char[urlLength+1];
			memset(url,'\0',urlLength+1);
			sprintf(url, "%s?usercategories=1&seconds=%s", URL, feed->getSeconds().c_str());
			res = mHttp.create(url, HTTP_GET);
		}
		else {
			//work out how long the url will be, the 4 is for the & and = symbols
			urlLength = 70 + URLSIZE + path[path.size()-1].length() + feed->getSeconds().length();
			url = new char[urlLength+1];
			memset(url,'\0',urlLength+1);
			sprintf(url, "%s?usersubcategories=1&category=%s&seconds=%s", URL, path[path.size()-1].c_str(), feed->getSeconds().c_str());
			res = mHttp.create(url, HTTP_GET);
		}

		if(res < 0) {
			notice->setCaption("");
		} else {
			mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
			mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
			feed->addHttp();
			mHttp.finish();

		}

		if (url != NULL) {
			delete url;
			url = NULL;
		}
	}
}

void AlbumLoadScreen::httpFinished(MAUtil::HttpConnection* http, int result) {
	if (result == 200) {
		xmlConn = XmlConnection::XmlConnection();
		xmlConn.parse(http, this, this);
	} else {
		mHttp.close();
		notice->setCaption("");
		feed->remHttp();
	}
}

void AlbumLoadScreen::connReadFinished(Connection* conn, int result) {}

void AlbumLoadScreen::xcConnError(int code) {
	feed->remHttp();
	if (code == -6) {
		return;
	} else {

	}
}

void AlbumLoadScreen::mtxEncoding(const char* ) {
}

void AlbumLoadScreen::mtxTagStart(const char* name, int len) {
	if (!strcmp(name, "usercategories") || !strcmp(name, "categories")) {
		album->clearAll();
	}
	parentTag = name;
}

void AlbumLoadScreen::mtxTagAttr(const char* attrName, const char* attrValue) {

}

void AlbumLoadScreen::mtxTagData(const char* data, int len) {
	if (!strcmp(parentTag.c_str(), "usercategories")) {
		//album->clearAll();
	} else if(!strcmp(parentTag.c_str(), "albumname")) {
		temp1 += data;
	} else if(!strcmp(parentTag.c_str(), "albumid")) {
		temp += data;
	} else if(!strcmp(parentTag.c_str(), "error")) {
		error_msg += data;
	} else if(!strcmp(parentTag.c_str(), "categoryname")) {
		temp1 += data;
	} else if(!strcmp(parentTag.c_str(), "categoryid")) {
		temp += data;
	} else if(!strcmp(parentTag.c_str(), "gamedescription")) {
		temp1 += data;
	} else if(!strcmp(parentTag.c_str(), "gameid")) {
		temp += data;
	} else if (!strcmp(parentTag.c_str(), "hascards")) {
		hasCards += data;
	} else if (!strcmp(parentTag.c_str(), "updated")) {
		updated += data;
	}
}

void AlbumLoadScreen::mtxTagEnd(const char* name, int len) {
	if(!strcmp(name, "album") || !strcmp(name, "categoryname") || !strcmp(name, "gamedescription")) {
		notice->setCaption("");
		album->addAlbum(temp.c_str(), temp1.c_str(), (hasCards=="true"), (updated=="1"));
		temp = "";
		hasCards = "";
		updated = "";
		temp1 = "";
	} else if (!strcmp(name, "usercategories") || !strcmp(name, "categories") || !strcmp(name, "games")) {
		switch (screenType) {
			default:
				notice->setCaption("");
				break;
		}
		if (screenType == ST_ALBUMS) {
			String al = album->getAll();
			if (path.size() == 0) {
				this->feed->getAlbum()->setAll(al.c_str());
				Util::saveData("lb.sav", al.c_str());
			}
			else {
				char *file = new char[path[path.size()-1].length() + 6];
				memset(file, 0, path.end()->length() + 6);
				sprintf(file, "a%s.sav", path[path.size()-1].c_str());
				Util::saveData(file, al.c_str());
				delete file;
			}
			al = "";
		}
		drawList();
		if ((album->size() == 1)&&(screenType != ST_ALBUMS)) {
			Vector<String> display = album->getNames();
			Album* val = album->getAlbum(display.begin()->c_str());
			if (val != NULL) {
				if (next != NULL) {
					delete next;
					feed->remHttp();
					next = NULL;
				}
				switch (screenType) {
					case ST_ALBUMS:
						if (val->getHasCards()) {
							if (strcmp(val->getId().c_str(), "-3") == 0) {
								next = new AlbumViewScreen(this, feed, val->getId(), AlbumViewScreen::AT_NEW_CARDS);
								next->show();
							}
							else {
								next = new AlbumViewScreen(this, feed, val->getId(), AlbumViewScreen::AT_NORMAL);
								next->show();
							}
						}
						else {
							//if a category has no cards, it means it has sub categories.
							//it is added to the path so we can back track
							path.add(val->getId());
							//then it must be loaded
							loadCategory();
						}
						break;
					}
				}
				display.clear();
			}
		temp = "";
		hasCards = "";
		updated = "";
		temp1 = "";
		error_msg = "";
	} else if(!strcmp(name, "error")) {
		notice->setCaption(error_msg.c_str());
		temp = "";
		hasCards = "";
		updated = "";
		temp1 = "";
		error_msg = "";
	} else {
		notice->setCaption("");
	}
}

void AlbumLoadScreen::mtxParseError(int) {}

void AlbumLoadScreen::mtxEmptyTagEnd() {
}

void AlbumLoadScreen::mtxTagStartEnd() {
}

int AlbumLoadScreen::getCount() {
	return size;
}

void AlbumLoadScreen::setDeckId(String d) {
	deckId = d;
}
