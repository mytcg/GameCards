#include <conprint.h>

#include "TutorialScreen.h"
#include "../utils/Util.h"
#include "../UI/Button.h"

TutorialScreen::TutorialScreen(MainScreen *previous, Vector<String> tutUrls) {
	lprintfln("TutorialScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	this->previous = previous;

	currentSelectedKey = NULL;
	currentKeyPosition = -1;

	index = 0;
	itemCount = tutUrls.size();
	imageCache = new ImageCache();
	mainLayout = new Layout(0, 0, scrWidth, scrHeight, NULL, 1, 2);
	mainLayout->setSkin(Util::getSkinBack());
	mainLayout->setDrawBackground(true);

	softKeys = Util::createSoftKeyBar(Util::getSoftKeyBarHeight(), "", "Terug", "");
	height = softKeys->getHeight();
	Layout *subLayout = new Layout(0, 0, scrWidth, scrHeight-height, mainLayout, 3, 1);
	subLayout->setPaddingLeft(PADDING);
	subLayout->setPaddingRight(PADDING);
	subLayout->setDrawBackground(false);

	leftArrow = new Image(0, 0, ARROW_WIDTH, subLayout->getHeight(), subLayout, false, false, RES_UNSELECT_ICON);
	leftArrow->setDrawBackground(false);

	midList = new ListBox(0, 0, scrWidth - (ARROW_WIDTH * 2) - (PADDING*2), subLayout->getHeight(), subLayout);
	midList->setDrawBackground(false);

	rightArrow = new Image(0, 0, ARROW_WIDTH, subLayout->getHeight(), subLayout, false, false, RES_RIGHT_ARROW);
	rightArrow->setDrawBackground(false);

	MobImage *tempImage;
	for(int i = 0; i < itemCount; i++){
		tempImage = new MobImage(0, 0, scrWidth - (ARROW_WIDTH * 2) - (PADDING*2), subLayout->getHeight(), NULL);
		tempImage->setResource(Util::loadImageFromResource(portrait?RES_LOADING1:RES_LOADING_FLIP1));
		tempImage->setDrawBackground(false);

		Util::retrieveImage(tempImage, getIdFromImageString(tutUrls[i]), tutUrls[i], scrHeight-height-PADDING*2, imageCache, 7);

		tutImages.add(tempImage);
	}
	tempImage = NULL;

	midList->add(tutImages[0]);

	mainLayout->add(softKeys);

	int capLength = 6 + Util::intlen(index+1) + Util::intlen(itemCount);
	char *cap = new char[capLength+1];
	memset(cap,'\0',capLength+1);
	sprintf(cap, "Page %d/%d", index+1, itemCount);
	((Button*)(softKeys->getChildren()[1]))->setCaption(cap);
	delete cap;

	this->setMain(mainLayout);
}

String TutorialScreen::getIdFromImageString(String imageUrl) {
	int lastSlash = imageUrl.findLastOf('/');
	int lastDot = imageUrl.findLastOf('.');
	return imageUrl.substr(lastSlash + 1, lastDot - lastSlash - 1);
}

TutorialScreen::~TutorialScreen() {
	lprintfln("~TutorialScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	delete mainLayout;
	mainLayout = NULL;
	if (imageCache != NULL) {
		delete imageCache;
		imageCache = NULL;
	}
	previous = NULL;
	leftArrow = NULL;
	rightArrow = NULL;
	midList = NULL;
	itemCount = 0;

	for (int i = 0; i < tutImages.size(); i++) {
		if (i != index) {
			delete tutImages[i];
		}
		tutImages[i] = NULL;
	}
	tutImages.clear();

	index = 0;

	currentSelectedKey = NULL;
	currentKeyPosition = -1;
}

void TutorialScreen::locateItem(MAPoint2d point) {
	panLeft = false;
	panRight = false;
	left = false;
	right = false;

	Point p;
	p.set(point.x, point.y);

	if (leftArrow->contains(p)) {
		panLeft = true;
		return;
	}
	else if (rightArrow->contains(p)) {
		panRight = true;
		return;
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

void TutorialScreen::pointerPressEvent(MAPoint2d point) {
	pointPressed = point;
    locateItem(point);
}

void TutorialScreen::pointerMoveEvent(MAPoint2d point) {
    locateItem(point);
}

void TutorialScreen::pointerReleaseEvent(MAPoint2d point) {
	pointReleased = point;
	if (right) {
		keyPressEvent(MAK_SOFTRIGHT);
	}
	else if (left) {
		keyPressEvent(MAK_SOFTLEFT);
	}
	else if (panLeft) {
		keyPressEvent(MAK_LEFT);
	}
	else if (panRight) {
		keyPressEvent(MAK_RIGHT);
	}
}

void TutorialScreen::keyPressEvent(int keyCode) {
	switch (keyCode) {
		case MAK_LEFT:
			if (currentSelectedKey == NULL) {
				scrollImage(-1);
			}
			break;
		case MAK_RIGHT:
			if (currentSelectedKey == NULL) {
				scrollImage(1);
			}
			break;
		case MAK_SOFTLEFT:
			break;
		case MAK_BACK:
		case MAK_SOFTRIGHT:
			previous->show();
			break;
		case MAK_FIRE:
			if (currentSelectedKey != NULL && currentKeyPosition == 2) {
				previous->show();
			}
			break;
		case MAK_UP:
			if(currentSelectedKey!=NULL){
				currentSelectedKey->setSelected(false);
				currentSelectedKey = NULL;
				currentKeyPosition = -1;
			}
			break;
		case MAK_DOWN:
			for(int i = 0; i < softKeys->getChildren().size();i++){
				if(((Button *)softKeys->getChildren()[i])->isSelectable()){
					currentKeyPosition = i;
					currentSelectedKey = softKeys->getChildren()[i];
					currentSelectedKey->setSelected(true);
					break;
				}
			}
			break;
	}
}

void TutorialScreen::scrollImage(int move) {
	if ((move != 0) && ((move < 0 && index > 0) || (move > 0 && index < (itemCount-1)))) {
		index = ((index+move)<0)?0:((index+move)>=itemCount)?itemCount-1:(index+move);

		midList->clear();
		midList->add(tutImages[index]);

		if (index == 0) {
			leftArrow->setResource(RES_UNSELECT_ICON);
		}
		else {
			leftArrow->setResource(RES_LEFT_ARROW);
		}

		if (index == itemCount-1) {
			rightArrow->setResource(RES_UNSELECT_ICON);
		}
		else {
			rightArrow->setResource(RES_RIGHT_ARROW);
		}

		int capLength = 6 + Util::intlen(index+1) + Util::intlen(itemCount);
		char *cap = new char[capLength+1];
		memset(cap,'\0',capLength+1);
		sprintf(cap, "Page %d/%d", index+1, itemCount);
		((Button*)(softKeys->getChildren()[1]))->setCaption(cap);
		delete cap;
	}
}
