#include <conprint.h>
#include <mastdlib.h>

#include "AchiDetailsScreen.h"
#include "ViewAchisScreen.h"
#include "../utils/Util.h"
#include "../utils/Albums.h"
#include "OptionsScreen.h"
#include "../utils/Album.h"
#include "../UI/Button.h"

ViewAchisScreen::ViewAchisScreen(MainScreen *previous, Feed *feed) : mHttp(this) {
	this->previous = previous;
	this->feed = feed;
	lprintfln("ViewAchisScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	size = 0;
	moved = 0;
	currentSelectedKey = NULL;
	currentKeyPosition = -1;
	int res = -1;
	char *url = NULL;
	int urlLength = 0;
	selectedList = 0;

	empt = true;
	shown = false;
	updated = "0";

	ViewAchisScreen::name = "";
	description = "";
	incompleteImage = "";
	progress = 0;
	target = 0;
	strProgress = "";
	strTarget = "";
	completeImage = "";
	dateCompleted = "";

	mImageCache = new ImageCache();

	next = NULL;
	mainLayout = Util::createMainLayout("", "Back");

	listBox = (ListBox*) mainLayout->getChildren()[0]->getChildren()[2];
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];

	notice->setCaption("Checking for achievements...");
	urlLength = 60 + URLSIZE;
	url = new char[urlLength+1];
	memset(url,'\0',urlLength+1);
	sprintf(url, "%s?getachis=1", URL);
	lprintfln("%s", url);
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

	drawList();

	if (url != NULL) {
		delete url;
		url = NULL;
	}
}

ViewAchisScreen::~ViewAchisScreen() {
	lprintfln("~ViewAchisScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	midListBox->clear();
	delete mainLayout;
	mainLayout = NULL;
	if(next!=NULL){
		delete next;
		next = NULL;
		feed->remHttp();
	}
	parentTag="";
	error_msg="";
	updated="";

	for (int i = 0; i < achievements.size(); i++) {
		delete achievements[i];
		achievements[i] = NULL;
	}
	achievements.clear();

	if (mImageCache != NULL) {
		delete mImageCache;
		mImageCache = NULL;
	}
}

void ViewAchisScreen::pointerPressEvent(MAPoint2d point)
{
	xStart = point.x;
    locateItem(point);
}

void ViewAchisScreen::pointerMoveEvent(MAPoint2d point)
{
	moved++;
    locateItem(point);
}

void ViewAchisScreen::pointerReleaseEvent(MAPoint2d point)
{
	if (!empt && (list || listLeft || listRight)) {
		int xEnd = point.x;
		int distance = abs(xEnd - xStart);

		if (distance >= (scrWidth * 0.4)) {
			moved=0;
			switchList(xEnd>xStart?-1:1);
			return;
		}
	}
	if (moved <= 8) {
		if (right) {
			keyPressEvent(MAK_SOFTRIGHT);
		} else if (left) {
			keyPressEvent(MAK_SOFTLEFT);
		} else if (list) {
			keyPressEvent(MAK_FIRE);
		} else if (listLeft) {
			keyPressEvent(MAK_LEFT);
		} else if (listRight) {
			keyPressEvent(MAK_RIGHT);
		}
	}
	moved=0;
}

void ViewAchisScreen::locateItem(MAPoint2d point)
{
	list = false;
	left = false;
	right = false;
	mid = false;
	listLeft = false;
	listRight = false;
	Point p;
	p.set(point.x, point.y);
	if (leftArrow->contains(p)) {
		listLeft = true;
		return;
	}
	else if (rightArrow->contains(p)) {
		listRight = true;
		return;
	}
	for(int i = 0; selectedList < achiLists.size() &&
		i < (achiLists[selectedList]->getChildren()).size(); i++)
	{
		if(achiLists[selectedList]->getChildren()[i]->contains(p))
		{
			list = true;
			achiLists[selectedList]->setSelectedIndex(i);
		}
	}
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

void ViewAchisScreen::drawList() {
	clearListBox();
	empt = false;
	listBox->clear();
	int ind = listBox->getSelectedIndex();
	int itemsPerList = listBox->getHeight() / ACHI_ITEM_HEIGHT;
	Layout *listLayout;
	if (itemsPerList < achievements.size()) {
		listLayout = new Layout(0, 0, listBox->getWidth(), listBox->getHeight(), listBox, 3, 1);
		listLayout->setDrawBackground(false);
		listLayout->setVerticalAlignment(Layout::VA_CENTER);

		leftArrow = new Image(0, 0, ARROW_WIDTH, listLayout->getHeight(), listLayout, false, false, RES_LEFT_ARROW);
		leftArrow->setDrawBackground(false);

		midListBox = new ListBox(0, 0, listLayout->getWidth() - (ARROW_WIDTH*2) - (PADDING*2), listLayout->getHeight(), listLayout, ListBox::LBO_VERTICAL);
		midListBox->setDrawBackground(false);

		rightArrow = new Image(0, 0, ARROW_WIDTH, listLayout->getHeight(), listLayout, false, false, RES_RIGHT_ARROW);
		rightArrow->setDrawBackground(false);
	} else {
		listLayout = new Layout(0, 0, listBox->getWidth(), listBox->getHeight(), listBox, 1, 1);
		listLayout->setDrawBackground(false);
		listLayout->setVerticalAlignment(Layout::VA_CENTER);

		leftArrow = new Image(0, 0, ARROW_WIDTH, listLayout->getHeight(), NULL, false, false, RES_LEFT_ARROW);
		leftArrow->setDrawBackground(false);

		midListBox = new ListBox(0, 0, listLayout->getWidth() - (PADDING*2), listLayout->getHeight(), listLayout, ListBox::LBO_VERTICAL);
		midListBox->setDrawBackground(false);

		rightArrow = new Image(0, 0, ARROW_WIDTH, listLayout->getHeight(), NULL, false, false, RES_RIGHT_ARROW);
		rightArrow->setDrawBackground(false);
	}
	int currentList = -1;
	ListBox *tempList = NULL;
	Layout *feedLayout = NULL;
	int i = 0;
	String achiName = "";
	size = 0;
	for(int i = 0; i < achievements.size(); i++) {
		achiName = achievements[i]->getName();
		if (i % itemsPerList == 0) {
			tempList = new ListBox(0, 0, midListBox->getWidth(), midListBox->getHeight(), NULL);
			tempList->setOrientation(ListBox::LBO_VERTICAL);
			tempList->setDrawBackground(false);
			currentList++;
			achiLists.add(tempList);
		}

		feedLayout = new Layout(0, 0, tempList->getWidth()-(PADDING*2), ACHI_ITEM_HEIGHT + ((midListBox->getHeight() % THUMB_HEIGHT) / itemsPerList), tempList, 2, 1);
		feedLayout->setSkin(Util::getSkinAlbum());
		feedLayout->setDrawBackground(true);
		feedLayout->addWidgetListener(this);
		feedLayout->setVerticalAlignment(Layout::VA_CENTER);
		feedLayout->setPaddingTop(PADDING);

		//if there are achievements, the images must be downloaded
		MobImage *tempImage = new MobImage(0, 0, ACHI_ITEM_HEIGHT, 66, feedLayout, false, false, Util::loadImageFromResource(RES_LOADINGTHUMB));
		Util::retrieveImage(tempImage, getIdFromImageString(achievements[i]->getDisplayImage()), achievements[i]->getDisplayImage(), 66, mImageCache);

		label = new Label(0,0, scrWidth-86, ACHI_ITEM_HEIGHT, feedLayout, achiName, 0, Util::getDefaultFont());

		tempImage->setDrawBackground(false);
		label->setDrawBackground(false);
		label->setVerticalAlignment(Label::VA_CENTER);
		label->setAutoSizeY();
		label->setAutoSizeX();
		label->setMultiLine();
		//lprintfln("getIdFromImageString(achievements[i]->getDisplayImage()): %s", getIdFromImageString(achievements[i]->getDisplayImage()).c_str());

		size++;
		achiName = "";
	}
	feedLayout = NULL;
	achiName="";
	if (achievements.size() >= 1) {
		int listIndex = ind / itemsPerList;
		int listItem = ind % itemsPerList;
		selectedList = listIndex;
		midListBox->add(achiLists[listIndex]);
		achiLists[listIndex]->setSelectedIndex(listItem);
		achiLists[listIndex]->getChildren()[achiLists[listIndex]->getSelectedIndex()]->setSelected(true);
	} else {
		empt = true;
		midListBox->add(Util::createSubLabel("Empty"));
		midListBox->setSelectedIndex(0);
		size++;
	}
	int capLength = 6 + Util::intlen((selectedList + 1)) + Util::intlen(achiLists.size());
	char *cap = new char[capLength+1];
	memset(cap,'\0',capLength+1);
	sprintf(cap, "Page %d/%d", (selectedList + 1), achiLists.size());

	if (shown) {
		((Label*)this->getMain()->getChildren()[1]->getChildren()[1])->setCaption(cap);
	}
	delete cap;
}

void ViewAchisScreen::clearListBox() {
	Vector<Widget*> tempWidgets;

	if (!empt) {
		midListBox->clear();
		for (int i = 0; i < achiLists.size(); i++) {
			tempWidgets.add(achiLists[i]);
		}
		achiLists.clear();
		for (int i = 0; i < listBox->getChildren().size(); i++) {
			tempWidgets.add(listBox->getChildren()[i]);
		}
		listBox->clear();
		listBox->getChildren().clear();
	}
	else {
		for (int i = 0; i < listBox->getChildren().size(); i++) {
			tempWidgets.add(listBox->getChildren()[i]);
		}
		listBox->clear();
		listBox->getChildren().clear();
	}

	for (int j = 0; j < tempWidgets.size(); j++) {
		delete tempWidgets[j];
		tempWidgets[j] = NULL;
	}
	tempWidgets.clear();
}

void ViewAchisScreen::selectionChanged(Widget *widget, bool selected) {
	if(selected) {
		((Label *)widget->getChildren()[1])->setFont(Util::getDefaultSelected());
	} else {
		((Label *)widget->getChildren()[1])->setFont(Util::getDefaultFont());
	}
}

void ViewAchisScreen::show() {
	shown = true;
	if (listBox->getChildren().size() > 0) {
		if (empt){
			listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(true);
		}
		else {
			achiLists[selectedList]->getChildren()[achiLists[selectedList]->getSelectedIndex()]->setSelected(true);
		}
	}
	Screen::show();

	int capLength = 6 + Util::intlen((selectedList + 1)) + Util::intlen(achiLists.size());
	char *cap = new char[capLength+1];
	memset(cap,'\0',capLength+1);
	sprintf(cap, "Page %d/%d", (selectedList + 1), achiLists.size());
	((Label*)this->getMain()->getChildren()[1]->getChildren()[1])->setCaption(cap);
	delete cap;
}

void ViewAchisScreen::hide() {
	shown = false;
	if (listBox->getChildren().size() > 0) {
		if (empt){
			listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(false);
		}
		else {
			achiLists[selectedList]->getChildren()[achiLists[selectedList]->getSelectedIndex()]->setSelected(false);
		}
	}
	Screen::hide();
}

void ViewAchisScreen::keyPressEvent(int keyCode) {
	int selected = (achiLists[0]->getChildren().size() * selectedList) + achiLists[selectedList]->getSelectedIndex();
	int max = 0;
	int index = 0;
	if (empt) {
		max = midListBox->getChildren().size();
		index = midListBox->getSelectedIndex();
	}
	else {
		max = achiLists[selectedList]->getChildren().size();
		index = achiLists[selectedList]->getSelectedIndex();
	}
	Widget *currentSoftKeys = mainLayout->getChildren()[mainLayout->getChildren().size() - 1];
	switch(keyCode) {
		case MAK_UP:
			if(currentSelectedKey!=NULL){
				currentSelectedKey->setSelected(false);
				currentSelectedKey = NULL;
				currentKeyPosition = -1;
				if (empt) {
					midListBox->getChildren()[max-1]->setSelected(true);
				}
				else {
					achiLists[selectedList]->getChildren()[max-1]->setSelected(true);
				}
			}
			else {
				if (empt) {
					midListBox->selectPreviousItem();
				}
				else {
					achiLists[selectedList]->selectPreviousItem();
				}
			}

			break;
		case MAK_DOWN:
			if (index+1 < max) {
				if (empt) {
					midListBox->selectNextItem();
				}
				else {
					achiLists[selectedList]->selectNextItem();
				}
			} else {
				if (empt) {
					midListBox->getChildren()[index]->setSelected(false);
				}
				else {
					achiLists[selectedList]->getChildren()[index]->setSelected(false);
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
			} else if (!empt) {
				switchList(1);
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
			} else if (!empt) {
				switchList(-1);
			}
			break;
		case MAK_BACK:
		case MAK_SOFTRIGHT:
			previous->show();
			break;
		case MAK_SOFTLEFT:
		case MAK_FIRE:
			if(currentSoftKeys->getChildren()[0]->isSelected()){
				keyPressEvent(MAK_SOFTLEFT);
			}else if(currentSoftKeys->getChildren()[2]->isSelected()){
				keyPressEvent(MAK_SOFTRIGHT);
			}else if (!empt) {
				if (next != NULL) {
					delete next;
					next = NULL;
				}
				next = new AchiDetailsScreen(this, achievements[achiLists[selectedList]->getSelectedIndex()]);
				next->show();
			}
			break;
	}
}

void ViewAchisScreen::switchList(int nextOrPrev) {
	int currentIndex = achiLists[selectedList]->getSelectedIndex();

	if ((selectedList + nextOrPrev >= 0) && (selectedList + nextOrPrev < achiLists.size())) {
		selectedList += nextOrPrev;
	}
	else if (selectedList + nextOrPrev < 0) {
		selectedList = achiLists.size() - 1;
	}
	else if (selectedList + nextOrPrev >= achiLists.size()) {
		selectedList = 0;
	}
	midListBox->clear();
	midListBox->add(achiLists[selectedList]);

	currentIndex = currentIndex>=achiLists[selectedList]->getChildren().size()?achiLists[selectedList]->getChildren().size()-1:currentIndex;

	achiLists[selectedList]->setSelectedIndex(currentIndex);
	achiLists[selectedList]->getChildren()[currentIndex]->setSelected(true);

	int capLength = 6 + Util::intlen((selectedList + 1)) + Util::intlen(achiLists.size());
	char *cap = new char[capLength+1];
	memset(cap,'\0',capLength+1);
	sprintf(cap, "Page %d/%d", (selectedList + 1), achiLists.size());
	((Label*)this->getMain()->getChildren()[1]->getChildren()[1])->setCaption(cap);
	delete cap;

	currentIndex = 0;
}

String ViewAchisScreen::getIdFromImageString(String imageUrl) {
	int lastSlash = imageUrl.findLastOf('/');
	int lastDot = imageUrl.findLastOf('.');
	return imageUrl.substr(lastSlash + 1, lastDot - lastSlash - 1);
}

void ViewAchisScreen::httpFinished(MAUtil::HttpConnection* http, int result) {
	if (result == 200) {
		xmlConn = XmlConnection::XmlConnection();
		xmlConn.parse(http, this, this);
	} else {
		mHttp.close();
		notice->setCaption("");
		feed->remHttp();
	}
}

void ViewAchisScreen::connReadFinished(Connection* conn, int result) {}

void ViewAchisScreen::xcConnError(int code) {
	feed->remHttp();
	if (code == -6) {
		return;
	} else {

	}
}

void ViewAchisScreen::mtxEncoding(const char* ) {
}

void ViewAchisScreen::mtxTagStart(const char* name, int len) {
	if (!strcmp(name, "achi")) {
		tempAchi = new Achievement();
	}
	parentTag = name;
}

void ViewAchisScreen::mtxTagAttr(const char* attrName, const char* attrValue) {

}

void ViewAchisScreen::mtxTagData(const char* data, int len) {
	if (!strcmp(parentTag.c_str(), "name")) {
		name += data;
	}
	else if (!strcmp(parentTag.c_str(), "description")) {
		description += data;
	}
	else if (!strcmp(parentTag.c_str(), "incomplete_image")) {
		incompleteImage += data;
	}
	else if (!strcmp(parentTag.c_str(), "progress")) {
		strProgress += data;
	}
	else if (!strcmp(parentTag.c_str(), "target")) {
		strTarget += data;
	}
	else if (!strcmp(parentTag.c_str(), "complete_image")) {
		completeImage += data;
	}
	else if (!strcmp(parentTag.c_str(), "date_completed")) {
		dateCompleted += data;
	}
	else if(!strcmp(parentTag.c_str(), "error")) {
		error_msg += data;
	}
}

void ViewAchisScreen::mtxTagEnd(const char* name, int len) {
	if (!strcmp(name, "achi")) {
		tempAchi->setName(ViewAchisScreen::name);
		tempAchi->setDescription(description);
		tempAchi->setIncompleteImage(incompleteImage);

		achievements.add(tempAchi);

		ViewAchisScreen::name = "";
		description = "";
		incompleteImage = "";
	}
	else if (!strcmp(name, "subachi")) {
		target = atoi(strTarget.c_str());
		progress = atoi(strProgress.c_str());

		tempAchi->addSubAchi(progress, target, completeImage, dateCompleted);

		progress = 0;
		target = 0;
		strProgress = "";
		strTarget = "";
		completeImage = "";
		dateCompleted = "";
	}
	else if (!strcmp(name, "achis")) {

		drawList();

		notice->setCaption("");
	}
}

void ViewAchisScreen::mtxParseError(int) {}

void ViewAchisScreen::mtxEmptyTagEnd() {
}

void ViewAchisScreen::mtxTagStartEnd() {
}
