#include <MAUtil/Graphics.h>
#include <conprint.h>

#include "MobFont.h"
#include "../../utils/Util.h"

MobFont::MobFont(MAHandle font, int rotation)
	: Font(font), rotation(rotation) {
}

short lineBreaks[2048]; // TODO: should probably change this to a vector (no good with limitations)
int numLineBreaks;

void MobFont::calcLineBreaks(const char* strS, int x, int y, const Rect& bound) const {
	int i = 0;
	int j = 0;
	int lastSpace = -1;
	MAPoint2d cursor = {x, y};
	CharDescriptor *chars = mCharset->chars;
	const unsigned char* str = (const unsigned char*)strS;

	while(str[i]) {
		if(str[i] == ' ') {
			lastSpace = i;
		} else if(str[i]=='\n') {

			cursor.x = x;
			cursor.y += mCharset->lineHeight + mLineSpacing;
			lineBreaks[j++] = i;
			i++;
			continue;
		}


		cursor.x += chars[str[i]].xAdvance;
		switch (rotation) {
			case TRANS_NONE:
				if(cursor.x > bound.x + bound.width) {
					if(lastSpace == -1) {
						lineBreaks[j++] = i;
					} else {
						lineBreaks[j++] = lastSpace+1;
						i = lastSpace+1;
						lastSpace = -1;
					}
					cursor.x = x + chars[str[i]].xAdvance;
				}
				break;
			case TRANS_ROT90:
				if(cursor.x > bound.y + bound.height) {
					if(lastSpace == -1) {
						lineBreaks[j++] = i;
					} else {
						lineBreaks[j++] = lastSpace+1;
						i = lastSpace+1;
						lastSpace = -1;
					}
					cursor.x = x + chars[str[i]].xAdvance;
				}
				break;
		}

		i++;
	}
	numLineBreaks = j;
	lineBreaks[j] = -1;
}

void MobFont::drawString(const char* strS, int x, int y) {
	if(!mFontImage) return;
	const unsigned char* str = (const unsigned char*)strS;
	MARect srcRect = {0, 0, 0, 0};
	MAPoint2d cursor = {x,y};

	CharDescriptor *chars = mCharset->chars;
	while(*str) {
		if((*str)=='\n')
		{
			cursor.x = x;
			cursor.y += mCharset->lineHeight + mLineSpacing;
			str++;
			continue;
		}

		srcRect.left = chars[*str].x;
		srcRect.top = chars[*str].y;
		srcRect.width = chars[*str].width;
		srcRect.height = chars[*str].height;

		MAPoint2d destPoint = {cursor.x + chars[*str].xOffset,
			cursor.y + chars[*str].yOffset};

		//maDrawImageRegion(this->mFontImage, &srcRect, &destPoint, 0);
		Gfx_drawImageRegion(this->mFontImage, &srcRect, &destPoint, rotation);

		cursor.x += chars[*str].xAdvance;
		str++;
	}
}

void MobFont::drawBoundedString(const char* strS, int x, int y, const Rect& bound) {
	int i = 0;
	int j = 0;
	if(!mFontImage) return;
	const unsigned char* str = (const unsigned char*)strS;
	calcLineBreaks(strS, x, y, bound);
	MARect srcRect = {0, 0, 0, 0};
	MAPoint2d cursor = {x, y};
	CharDescriptor *chars = mCharset->chars;
	MAPoint2d destPoint;
	while(str[i]) {
		if(lineBreaks[j] == i) {
			j++;
			cursor.x = x;
			cursor.y += mCharset->lineHeight + mLineSpacing;
			if(str[i]=='\n') {
				i++;
				continue;
			}
		}

		srcRect.left = chars[str[i]].x;
		srcRect.top = chars[str[i]].y;
		srcRect.width = chars[str[i]].width;
		srcRect.height = chars[str[i]].height;

		switch (rotation) {
			case TRANS_NONE:
				destPoint.x = cursor.x + chars[str[i]].xOffset;
				destPoint.y = cursor.y + chars[str[i]].yOffset;
				break;
			case TRANS_ROT90:
				destPoint.x = bound.width - (cursor.y + chars[str[i]].yOffset + chars[str[i]].height) + 4;
				destPoint.y = cursor.x + chars[str[i]].xOffset;
				break;
		}

		Gfx_drawImageRegion(this->mFontImage, &srcRect, &destPoint, rotation);

		cursor.x += chars[str[i]].xAdvance;
		i++;
	}
}

void MobFont::setRotation(int r) {
	rotation = r;
}
