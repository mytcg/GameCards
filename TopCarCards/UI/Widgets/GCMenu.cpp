#include <MAUI/Layout.h>
#include <mavsprintf.h>

#include "GCMenu.h"
#include "../../utils/Util.h"

GCMenu::GCMenu(item items[], int numItems, int x, int y, int width, int height,
		bool autosizeX, bool autosizeY, Widget* parent)
	:Widget(x, y, width, height, parent), items(items), numItems(numItems), enabled(false) {
	setEnabled(true);

	Layout *mainLayout = new Layout(0, 0, width, height, NULL, 1, 2);

	int imageHeight = height - 69;

	Layout *subLayout = new Layout(0, 0, width, imageHeight, mainLayout, 3, 1);
	subLayout->setPaddingLeft(5);
	subLayout->setPaddingRight(5);

	Image *arrow = new MobImage(0, 0, 13, imageHeight, subLayout, false, false, Util::loadImageFromResource(RES_LEFT_ARROW));
	mainImage = new MobImage(0, 0, width - 36, imageHeight, subLayout);
	arrow = new MobImage(0, 0, 13, imageHeight, subLayout, false, false, Util::loadImageFromResource(RES_RIGHT_ARROW));

	iconList = new KineticListBox(0, 0, width, height - imageHeight, mainLayout,
			KineticListBox::LBO_HORIZONTAL, KineticListBox::LBA_LINEAR, true);

	for (int i = 0; i < numItems; i++) {
		MobImage *tempImage = new MobImage(0, 0, 46 + (PADDING * 2), height - imageHeight, iconList);
		tempImage->setResource(Util::loadImageFromResource(items[i].icon));
	}
	if (iconList->getChildren().size() > 0) {
		emp = false;
		iconList->setSelectedIndex(0);
		iconList->getChildren()[0]->setSelected(true);
		mainImage->setResource(Util::loadImageFromResource(items[iconList->getSelectedIndex()].bigImage));
	}
	else {
		emp = true;
	}
	add(mainLayout);
}

GCMenu::~GCMenu() {
	for (int i = 0; i < children.size(); i++) {
		delete children[i];
	}

	items = NULL;
}

void GCMenu::setEnabled(bool e) {
	Environment& env = Environment::getEnvironment();
	if(!enabled && e) {
		env.addPointerListener(this);
		env.addKeyListener(this);
	}
	else if(enabled && !e) {
		env.removePointerListener(this);
		env.removeKeyListener(this);
	}

	enabled = e;
}

void GCMenu::selectNext() {
	if (!emp) {
		if (iconList->getSelectedIndex() + 1 >= iconList->getChildren().size()) {
			iconList->setSelectedIndex(0);
			iconList->getChildren()[0]->setSelected(true);
		}
		else {
			iconList->selectNextItem();
		}
		mainImage->setResource(Util::loadImageFromResource(items[iconList->getSelectedIndex()].bigImage));
	}
}

void GCMenu::selectPrevious() {
	if (!emp) {
		if (iconList->getSelectedIndex() - 1 < 0) {
			iconList->setSelectedIndex(iconList->getChildren().size() - 1);
			iconList->getChildren()[iconList->getSelectedIndex()]->setSelected(true);
		}
		else {
			iconList->selectPreviousItem();
		}
		mainImage->setResource(Util::loadImageFromResource(items[iconList->getSelectedIndex()].bigImage));
	}
}

void GCMenu::select(int i) {
	if (!emp && i < numItems) {
		iconList->setSelectedIndex(i);
		iconList->getChildren()[i]->setSelected(true);
		mainImage->setResource(Util::loadImageFromResource(items[iconList->getSelectedIndex()].bigImage));
	}
}

int GCMenu::getSelectedKey() {
	if (!emp) {
		return items[iconList->getSelectedIndex()].key;
	}
	return -1;
}

int GCMenu::getSelectedIndex() {
	if (!emp) {
		return iconList->getSelectedIndex();
	}
	return -1;
}

void GCMenu::keyPressEvent(int keyCode) {
	switch(keyCode) {
		case MAK_RIGHT:
			selectNext();
			break;
		case MAK_LEFT:
			selectPrevious();
			break;
	}
}

void GCMenu::pointerPressEvent(MAPoint2d point) {
	moved = 0;
}

void GCMenu::pointerMoveEvent(MAPoint2d point) {
	moved++;
}

void GCMenu::pointerReleaseEvent(MAPoint2d point) {
	if (moved < 8) {
		locateItem(point);
	}
}

void GCMenu::locateItem(MAPoint2d point) {
	Point p;
	p.set(point.x + ((iconList->getYOffset() >> 16) * -1), point.y);

	for (int i = 0; i < numItems; i++) {
		if(iconList->getChildren()[i]->contains(p))
		{
			select(i);
			return;
		}
	}
}

void GCMenu::drawWidget() {
}