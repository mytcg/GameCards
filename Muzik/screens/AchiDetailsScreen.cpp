#include <conprint.h>
#include <mastdlib.h>

#include "AchiDetailsScreen.h"
#include "../utils/Util.h"
#include "../utils/Albums.h"
#include "OptionsScreen.h"
#include "../utils/Album.h"
#include "../UI/Button.h"

AchiDetailsScreen::AchiDetailsScreen(MainScreen *previous, Achievement *achievement) : achi(achievement) {
	this->previous = previous;
	lprintfln("AchiDetailsScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	moved = 0;
	currentSelectedKey = NULL;
	currentKeyPosition = -1;

	mImageCache = new ImageCache();

	next = NULL;
	mainLayout = Util::createMainLayout("", "Terug", true);

	kinListBox = (KineticListBox*) mainLayout->getChildren()[0]->getChildren()[2];
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];

	title = new Label(0, 0, kinListBox->getWidth(), DEFAULT_LABEL_HEIGHT, kinListBox);
	title->setCaption(achi->getName());
	title->setDrawBackground(false);
	title->setHorizontalAlignment(Label::HA_CENTER);
	title->setFont(Util::getDefaultFont());
	title->setPaddingTop(PADDING*2);

	description = new Label(0, 0, kinListBox->getWidth(), 0, kinListBox);
	description->setCaption(achi->getDescription());
	description->setDrawBackground(false);
	description->setHorizontalAlignment(Label::HA_CENTER);
	description->setFont(Util::getDefaultFont());
	description->setMultiLine(true);
	description->setAutoSizeY(true);

	int tempTarget = 0, tempProgress = 0, completeCount = 0;
	bool complete = false;
	SubAchievement *tempSub = NULL;
	String progressString = "Current Progress: ";
	for (int i = 0; i < achi->subSize(); i++) {
		tempSub = achi->getSubAchi(i);
		if (tempSub->getDateCompleted().length() > 0) {
			complete = true;
			completeCount++;
		}
		else {
			tempTarget = tempSub->getTarget();
			tempProgress = tempSub->getProgress();

			complete = false;
		}
	}

	if (complete) {
		progressString += "Complete!";
	}
	else {
		int length = progressString.size() + Util::intlen(tempTarget) + Util::intlen(tempProgress) + 1;
		char *str = new char[length+1];
		memset(str,'\0',length+1);
		sprintf(str, "%s%d/%d", progressString.c_str(), tempProgress, tempTarget);

		progressString = str;

		delete str;
		str = NULL;
	}

	progress = new Label(0, 0, kinListBox->getWidth(), DEFAULT_LABEL_HEIGHT, kinListBox);
	progress->setCaption(progressString);
	progress->setDrawBackground(false);
	progress->setHorizontalAlignment(Label::HA_CENTER);
	progress->setFont(Util::getDefaultFont());
	progress->setPaddingTop(PADDING*2);

	if (completeCount > 0) {
		Layout *feedLayout = NULL;
		MobImage *tempImage = NULL;
		Label *label = new Label(0, 0, kinListBox->getWidth(), DEFAULT_LABEL_HEIGHT, kinListBox);
		label->setCaption("Badges Earned");
		label->setDrawBackground(false);
		label->setHorizontalAlignment(Label::HA_CENTER);
		label->setFont(Util::getDefaultFont());
		label->setPaddingTop(PADDING*2);

		for (int i = 0; i < completeCount; i++) {
			tempSub = achi->getSubAchi(i);

			feedLayout = new Layout(0, 0, kinListBox->getWidth()-(PADDING*2), ACHI_ITEM_HEIGHT, kinListBox, 2, 1);
			feedLayout->setDrawBackground(false);
			//feedLayout->addWidgetListener(this);
			feedLayout->setVerticalAlignment(Layout::VA_CENTER);

			//if there are achievements, the images must be downloaded
			tempImage = new MobImage(0, 0, 80, 66, feedLayout, false, false, Util::loadImageFromResource(RES_LOADINGTHUMB));
			tempImage->setDrawBackground(false);
			Util::retrieveImage(tempImage, getIdFromImageString(tempSub->getCompletedImage()), tempSub->getCompletedImage(), 66, mImageCache);

			label = new Label(0,0, scrWidth-86, ACHI_ITEM_HEIGHT, feedLayout, tempSub->getDateCompleted(), 0, Util::getDefaultFont());
			label->setDrawBackground(false);
			label->setVerticalAlignment(Label::VA_CENTER);
			label->setPaddingLeft(PADDING * 2);
		}

		feedLayout = NULL;
		tempImage = NULL;
		label = NULL;
	}

	kinListBox->setSelectedIndex(0);

	this->setMain(mainLayout);
}

AchiDetailsScreen::~AchiDetailsScreen() {
	lprintfln("~AchiDetailsScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	delete mainLayout;
	mainLayout = NULL;
	if(next!=NULL){
		delete next;
		next = NULL;
		feed->remHttp();
	}

	if (mImageCache != NULL) {
		delete mImageCache;
		mImageCache = NULL;
	}
}

void AchiDetailsScreen::pointerPressEvent(MAPoint2d point)
{
    locateItem(point);
}

void AchiDetailsScreen::pointerMoveEvent(MAPoint2d point)
{
	moved++;
    locateItem(point);
}

void AchiDetailsScreen::pointerReleaseEvent(MAPoint2d point)
{
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

void AchiDetailsScreen::locateItem(MAPoint2d point)
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
				moved=0;
				left = true;
			} else if (i == 1) {
				moved=0;
			} else if (i == 2) {
				moved=0;
				right = true;
			}
			return;
		}
	}
}

void AchiDetailsScreen::selectionChanged(Widget *widget, bool selected) {

}

void AchiDetailsScreen::keyPressEvent(int keyCode) {
	Widget *currentSoftKeys = mainLayout->getChildren()[mainLayout->getChildren().size() - 1];
	switch(keyCode) {
		case MAK_UP:
			if(currentSelectedKey!=NULL){
				currentSelectedKey->setSelected(false);
				currentSelectedKey = NULL;
				currentKeyPosition = -1;

				kinListBox->setSelectedIndex(kinListBox->getChildren().size() - 1);
			}
			else {
				kinListBox->selectPreviousItem();
			}

			break;
		case MAK_DOWN:
			if (kinListBox->getSelectedIndex() < kinListBox->getChildren().size() - 1) {
				kinListBox->selectNextItem();
			}
			else {
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
		case MAK_BACK:
		case MAK_SOFTRIGHT:
			previous->show();
			break;
		case MAK_SOFTLEFT:
		case MAK_FIRE:
			if(currentSoftKeys->getChildren()[0]->isSelected()){
				keyPressEvent(MAK_SOFTLEFT);
			} else if(currentSoftKeys->getChildren()[2]->isSelected()){
				keyPressEvent(MAK_SOFTRIGHT);
			}
			break;
	}
}

String AchiDetailsScreen::getIdFromImageString(String imageUrl) {
	int lastSlash = imageUrl.findLastOf('/');
	int lastDot = imageUrl.findLastOf('.');
	return imageUrl.substr(lastSlash + 1, lastDot - lastSlash - 1);
}
