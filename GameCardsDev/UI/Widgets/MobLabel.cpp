#include <conprint.h>

#include "MobLabel.h"

MobLabel::MobLabel(int x, int y, int width, int height, Widget* parent, int rotation)
	: Label(x, y, width, height, parent), rotation(rotation) {
}

void MobLabel::setFont(MobFont* font) {
	font->setRotation(rotation);
	Label::setFont(font);
}

void MobLabel::drawWidget() {
	const char* wStr = caption.c_str();
	int textX=0, textY=0;
	getTextStart(&textX, &textY);
	if(font) {

		if(multiLine) {
			Rect tempRect = Rect(0, 0, paddedBounds.width, paddedBounds.height);
			((MobFont*)font)->drawBoundedString(wStr, textX, textY, tempRect);
		} else {

			if(autoSizeX)
				((MobFont*)font)->drawString(wStr, textX, textY);
			else
				((MobFont*)font)->drawString(cuttedCaption.c_str(), textX, textY);
		}
	}
}
