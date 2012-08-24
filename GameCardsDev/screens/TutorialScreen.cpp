#include <conprint.h>

#include "TutorialScreen.h"
#include "../utils/Util.h"

TutorialScreen::TutorialScreen(MainScreen *previous, Vector<String> tutimages) : tutimages(tutimages) {
	lprintfln("TutorialScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	this->previous = previous;
	index = 0;
	itemCount = tutimages.size();
	imageCache = new ImageCache();
	mainLayout = new Layout(0, 0, scrWidth, scrHeight, NULL, 1, 2);
	mainLayout->setSkin(Util::getSkinBack());
	mainLayout->setDrawBackground(true);

	softKeys = Util::createSoftKeyBar(Util::getSoftKeyBarHeight(), "", "Back", "");
	height = softKeys->getHeight();
	Layout *subLayout = new Layout(0, 0, scrWidth, scrHeight-height, mainLayout, 3, 1);
	subLayout->setPaddingLeft(PADDING);
	subLayout->setPaddingRight(PADDING);
	subLayout->setDrawBackground(false);

	leftArrow = new Image(0, 0, ARROW_WIDTH, subLayout->getHeight(), subLayout, false, false, RES_UNSELECT_ICON);
	leftArrow->setDrawBackground(false);

	imge = new MobImage(0, 0, scrWidth - (ARROW_WIDTH * 2) - (PADDING*2), subLayout->getHeight(), subLayout, false, false, Util::loadImageFromResource(portrait?RES_LOADING1:RES_LOADING_FLIP1));
	imge->setDrawBackground(false);
	//imge->setResource(tutimages[0]);

	Util::retrieveImage(imge, getIdFromImageString(tutimages[0]), tutimages[0],scrHeight-height-PADDING*2, imageCache);

	rightArrow = new Image(0, 0, ARROW_WIDTH, subLayout->getHeight(), subLayout, false, false, RES_RIGHT_ARROW);
	rightArrow->setDrawBackground(false);

	MobImage *tempImage;
	for(int i = 1;i < itemCount; i++){
		tempImage = new MobImage(0, 0, scrWidth - (ARROW_WIDTH * 2) - (PADDING*2), subLayout->getHeight());
		Util::retrieveImage(tempImage, getIdFromImageString(tutimages[i]), tutimages[i],scrHeight-height-PADDING*2, imageCache);
	}
	delete tempImage;
	mainLayout->add(softKeys);

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
	imge = NULL;
	leftArrow = NULL;
	rightArrow = NULL;
	itemCount = 0, index = 0;
	tutimages.clear();
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
			scrollImage(-1);
			break;
		case MAK_RIGHT:
			scrollImage(1);
			break;
		case MAK_SOFTLEFT:
			//if (index == itemCount-1) {
			//	origMenu->show();
			//}
			break;
		case MAK_BACK:
		case MAK_SOFTRIGHT:
			previous->show();
			break;
	}
}

void TutorialScreen::scrollImage(int move) {
	if ((move != 0) && ((move < 0 && index > 0) || (move > 0 && index < (itemCount-1)))) {
		index = ((index+move)<0)?0:((index+move)>=itemCount)?itemCount-1:(index+move);
		//imge->setResource(tutimages[index]);
		Util::retrieveImage(imge, getIdFromImageString(tutimages[index]), tutimages[index],scrHeight-height-PADDING*2, imageCache);

		if (index == 0) {
			leftArrow->setResource(RES_UNSELECT_ICON);
		}
		else {
			leftArrow->setResource(RES_LEFT_ARROW);
		}

		if (index == itemCount-1) {
			rightArrow->setResource(RES_UNSELECT_ICON);
			Util::updateSoftKeyLayout("", "Back", "", mainLayout);
		}
		else {
			rightArrow->setResource(RES_RIGHT_ARROW);
			Util::updateSoftKeyLayout("", "Back", "", mainLayout);
		}
	}
}
