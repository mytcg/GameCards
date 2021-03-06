#include <conprint.h>
#include <MAUI/Layout.h>

#include "AlbumViewScreen.h"
#include "CompareScreen.h"
#include "OptionsScreen.h"
#include "../utils/Util.h"
#include "../UI/Button.h"

CompareScreen::CompareScreen(MainScreen *previous, MAHandle img, Feed *feed, bool flip, Card *card, Card *compare) :mHttp(this), img(img), flip(flip), card(card), compare(compare) {
	lprintfln("CompareScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	this->previous = previous;
	this->feed = feed;
	busy = false;
	next = NULL;
	currentSelectedStat = -1;
	flipOrSelect = 0;
	imageCache = new ImageCache();
	currentSelectedKey = NULL;
	currentKeyPosition = -1;
	if (card != NULL) {
		mainLayout = Util::createImageLayout("", "Terug", "Draai");
		kinListBox = (KineticListBox*) mainLayout->getChildren()[0];
		height = kinListBox->getHeight();
	}else{
		mainLayout = Util::createImageLayout("Terug");
		kinListBox = (KineticListBox*) mainLayout->getChildren()[0]->getChildren()[1];
		height = kinListBox->getHeight()-70;
	}
	if(portrait){
		subLayout = new Layout(0, 0, kinListBox->getWidth(), kinListBox->getHeight(), kinListBox, 1, 2);
		subLayout->setDrawBackground(false);
	}else{
		subLayout = new Layout(0, 0, kinListBox->getWidth(), kinListBox->getHeight(), kinListBox, 2, 1);
		subLayout->setDrawBackground(false);
	}
	if(portrait){
		imge = new MobImage(0, 0, subLayout->getWidth(), subLayout->getHeight()/2, subLayout, false, false, Util::loadImageFromResource(img));
		cmpge = new MobImage(0, 0, subLayout->getWidth(), subLayout->getHeight()/2, subLayout, false, false, Util::loadImageFromResource(img));
	}else{
		imge = new MobImage(0, 0, subLayout->getWidth()/2, subLayout->getHeight(), subLayout, false, false, Util::loadImageFromResource(img));
		cmpge = new MobImage(0, 0, subLayout->getWidth()/2, subLayout->getHeight(), subLayout, false, false, Util::loadImageFromResource(img));
	}
	this->setMain(mainLayout);
	if (card != NULL && compare != NULL) {
		if (flip) {
			Util::retrieveBackFlip(imge, card, height-PADDING*2, imageCache);
			Util::retrieveBackFlip(cmpge, compare, height-PADDING*2, imageCache);
		} else {
			Util::retrieveFrontFlip(imge, card, height-PADDING*2, imageCache);
			Util::retrieveFrontFlip(cmpge, compare, height-PADDING*2, imageCache);
		}
	}
	else {
		imageCache = NULL;
	}
}
void CompareScreen::pointerPressEvent(MAPoint2d point)
{
	pointPressed = point;
    locateItem(point);
}

void CompareScreen::pointerMoveEvent(MAPoint2d point)
{
    locateItem(point);
}

void CompareScreen::pointerReleaseEvent(MAPoint2d point)
{
	pointReleased = point;
	if (right) {
		keyPressEvent(MAK_SOFTRIGHT);
		return;
	} else if (left) {
		keyPressEvent(MAK_SOFTLEFT);
		return;
	}
	if (card != NULL) {
		if (list) {
			if(Util::absoluteValue(pointPressed.x-pointReleased.x) >imge->getWidth()/100*15||Util::absoluteValue(pointPressed.x-pointReleased.x) > 45){
				flipOrSelect = 1;
			} else {
				flipOrSelect = 0;
				currentSelectedStat = -1;
				for(int i = 0;i<card->getStats().size();i++){
					if(flip==card->getStats()[i]->getFrontOrBack()){
						if(imge->statContains(card->getStats()[i]->getLeft(),card->getStats()[i]->getTop(),card->getStats()[i]->getWidth(),card->getStats()[i]->getHeight(),point.x, point.y, MobImage::LANDSCAPE)){
							currentSelectedStat = i;
							break;
						}
					}
				}
			}
			keyPressEvent(MAK_FIRE);
		}
	}
}

void CompareScreen::locateItem(MAPoint2d point)
{
	list = false;
	left = false;
	right = false;

	Point p;
	p.set(point.x, point.y);
	if (card == NULL) {
		for(int i = 0; i < (this->getMain()->getChildren()[1]->getChildren()).size(); i++)
		{
			if(this->getMain()->getChildren()[1]->getChildren()[i]->contains(p))
			{
				if (i == 0) {
					left = true;
				} else if (i == 1) {
					list = true;
				} else if (i == 2) {
					right = true;
				}
				return;
			}
		}
	} else {
		for(int i = 0; i < (this->getMain()->getChildren()).size(); i++)
		{
			if(this->getMain()->getChildren()[i]->contains(p))
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
				} else if (i == 1) {
					list = true;
				} else if (i == 2) {
					right = true;
				}
				return;
			}
		}
	}
}
CompareScreen::~CompareScreen() {
	lprintfln("~CompareScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	if (imge->getResource() != NULL) {
		maDestroyObject(imge->getResource());
	}
	clearListBox();
	kinListBox->clear();
	delete mainLayout;
	mainLayout = NULL;
	img = -1;
	if (next != NULL) {
		delete next;
		feed->remHttp();
		next = NULL;
	}
	if (imageCache != NULL) {
		delete imageCache;
		imageCache = NULL;
	}
}

void CompareScreen::clearListBox() {
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

void CompareScreen::keyPressEvent(int keyCode) {
	Widget *currentSoftKeys = mainLayout->getChildren()[mainLayout->getChildren().size() - 1];
	switch (keyCode) {
		case MAK_FIRE:
			if(currentSoftKeys->getChildren()[0]->isSelected()){
				keyPressEvent(MAK_SOFTLEFT);
				break;
			}else if(currentSoftKeys->getChildren()[2]->isSelected()){
				keyPressEvent(MAK_SOFTRIGHT);
				break;
			}
			currentSelectedKey = NULL;
			currentKeyPosition = -1;
			Util::updateSoftKeyLayout("", "Terug", "Draai", mainLayout);
			currentSoftKeys = mainLayout->getChildren()[mainLayout->getChildren().size() - 1];
			currentSelectedKey= currentSoftKeys->getChildren()[1];
			currentKeyPosition= 1;
			currentSoftKeys->getChildren()[1]->setSelected(true);

			imge->refreshWidget();
			imge->statAdded = false;
			currentSelectedStat = -1;

			flip=!flip;
			if (imge->getResource() != NULL) {
				maDestroyObject(imge->getResource());
			}
			if (cmpge->getResource() != NULL) {
				maDestroyObject(cmpge->getResource());
			}
			imge->setResource(Util::loadImageFromResource(portrait?RES_LOADING_FLIP1:RES_LOADING1));
			imge->update();
			imge->requestRepaint();

			cmpge->setResource(Util::loadImageFromResource(portrait?RES_LOADING_FLIP1:RES_LOADING1));
			cmpge->update();
			cmpge->requestRepaint();

			maUpdateScreen();
			if (flip) {
				Util::retrieveBackFlip(imge, card, height-PADDING*2, imageCache);
				Util::retrieveBackFlip(cmpge, compare, height-PADDING*2, imageCache);
			} else {
				Util::retrieveFrontFlip(imge, card, height-PADDING*2, imageCache);
				Util::retrieveFrontFlip(cmpge, compare, height-PADDING*2, imageCache);
			}
			currentSelectedStat = -1;
			break;
		case MAK_UP:
			if(currentSelectedKey!=NULL){
				currentSelectedKey->setSelected(false);
				currentSelectedKey = NULL;
				currentKeyPosition = -1;
			}
			break;
		case MAK_DOWN:
			if(currentSelectedKey==NULL){
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
			} else{
				Util::updateSoftKeyLayout("", "Terug", "Draai", mainLayout);
				imge->refreshWidget();
				imge->statAdded = false;
				currentSelectedStat = -1;
				flip=!flip;
				if (imge->getResource() != NULL) {
					maDestroyObject(imge->getResource());
				}
				if (cmpge->getResource() != NULL) {
					maDestroyObject(cmpge->getResource());
				}
				imge->setResource(Util::loadImageFromResource(portrait?RES_LOADING_FLIP1:RES_LOADING1));
				imge->update();
				imge->requestRepaint();
				cmpge->setResource(Util::loadImageFromResource(portrait?RES_LOADING_FLIP1:RES_LOADING1));
				cmpge->update();
				cmpge->requestRepaint();

				maUpdateScreen();
				if (flip) {
					Util::retrieveBackFlip(imge, card, height-PADDING*2, imageCache);
					Util::retrieveBackFlip(cmpge, compare, height-PADDING*2, imageCache);
				} else {
					Util::retrieveFrontFlip(imge, card, height-PADDING*2, imageCache);
					Util::retrieveFrontFlip(cmpge, compare, height-PADDING*2, imageCache);
				}
				currentSelectedStat = -1;
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
			} else{
				Util::updateSoftKeyLayout("", "Terug", "Draai", mainLayout);
				imge->refreshWidget();
				imge->statAdded = false;
				currentSelectedStat = -1;

				flip=!flip;
				if (imge->getResource() != NULL) {
					maDestroyObject(imge->getResource());
				}
				if (cmpge->getResource() != NULL) {
					maDestroyObject(cmpge->getResource());
				}
				imge->setResource(Util::loadImageFromResource(portrait?RES_LOADING_FLIP1:RES_LOADING1));
				imge->update();
				imge->requestRepaint();

				cmpge->setResource(Util::loadImageFromResource(portrait?RES_LOADING_FLIP1:RES_LOADING1));
				cmpge->update();
				cmpge->requestRepaint();
				maUpdateScreen();
				if (flip) {
					Util::retrieveBackFlip(imge, card, height-PADDING*2, imageCache);
					Util::retrieveBackFlip(cmpge, compare, height-PADDING*2, imageCache);
				} else {
					Util::retrieveFrontFlip(imge, card, height-PADDING*2, imageCache);
					Util::retrieveFrontFlip(cmpge, compare, height-PADDING*2, imageCache);
				}
				currentSelectedStat = -1;
			}
			break;
		case MAK_SOFTLEFT:
			break;
		case MAK_BACK:
		case MAK_SOFTRIGHT:
			previous->show();
			break;
	}
}

void CompareScreen::httpFinished(MAUtil::HttpConnection* http, int result) {
	if (result == 200) {
		xmlConn = XmlConnection::XmlConnection();
		xmlConn.parse(http, this, this);
	} else {
		mHttp.close();
		feed->remHttp();
	}
}

void CompareScreen::connReadFinished(Connection* conn, int result) {}

void CompareScreen::xcConnError(int code) {
	feed->remHttp();
	if (code == -6) {
		return;
	} else {
		//TODO handle error
	}
}

void CompareScreen::mtxEncoding(const char* ) {
}

void CompareScreen::mtxTagStart(const char* name, int len) {
	parentTag = name;
}

void CompareScreen::mtxTagAttr(const char* attrName, const char* attrValue) {
}

void CompareScreen::mtxTagData(const char* data, int len) {
}

void CompareScreen::mtxTagEnd(const char* name, int len) {
	if(!strcmp(name, "result")) {
		((AlbumViewScreen *)previous)->refresh();
	}
}

void CompareScreen::mtxParseError(int) {
}

void CompareScreen::mtxEmptyTagEnd() {
}

void CompareScreen::mtxTagStartEnd() {
}
