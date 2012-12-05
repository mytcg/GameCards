#include <MAUtil/Graphics.h>
#include <conprint.h>

#include "../../MAHeaders.h"
#include "../../utils/Util.h"
#include "MobImage.h"

MobImage::MobImage(int x, int y, int width, int height,
		Widget* parent, bool autosizeX, bool autosizeY, MAHandle res, bool front)
	: Image(x, y, width, height, parent, autosizeX, autosizeY, res), front(front) {
	statAdded = false;
	hasNote = false;
	refreshing = false;
	drawRect = false;
}

MobImage::~MobImage() {
	clearStats();

	if (getResource() != NULL) {
		maDestroyObject(getResource());
	}
	resource = NULL;
}

void MobImage::setResource(MAHandle res) {
	if (getResource() != NULL) {
		maDestroyObject(getResource());
	}
	Image::setResource(res);
}

void MobImage::setHasNote(bool n) {
	hasNote = n;
}

void MobImage::drawRectangle(int x, int y, int width, int height){
	Gfx_line(x, y, x+width, y);
	Gfx_line(x, y, x, y+height);
	Gfx_line(x+width, y, x+width, y+height);
	Gfx_line(x, y+height, x+width, y+height);
}

bool MobImage::statContains(int x, int y, int width, int height, int pointX, int pointY, int orientation){
	Rect *r = NULL;
	switch(orientation) {
		case LANDSCAPE:
			r = new Rect((((paddedBounds.width >> 1) + (imageWidth >> 1))-((y*imageWidth/350)+height*imageWidth/350))+3,
					(((paddedBounds.height >> 1) - (imageHeight >> 1))+getPosition().y)+(x*imageHeight/250),
					height*imageWidth/350,
					width*imageHeight/250);
			break;
		case PORTRAIT:
			r = new Rect((x*this->getWidth()/250),(y*this->getHeight()/350),width*this->getWidth()/250,height*this->getHeight()/350);
			break;
	}

	return (r==NULL?false:r->contains(pointX, pointY));
}

void MobImage::selectStat(int x, int y, int width, int height, int red, int green, int blue, int orientation){

	drawRect = true;

	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_red = red;
	_green = green;
	_blue = blue;
	_orientation = orientation;
	statAdded = true;

	//this->setDirty(true);

	//this->requestRepaint();

	//Gfx_setColor(red,green,blue);
	//Gfx_clearMatrix();
	//drawRectangle((x*this->getWidth()/250),(y*this->getHeight()/350),width*this->getWidth()/250,height*this->getHeight()/350);
	//code for portrait
	/*if(portrait==1){
		switch(_orientation) {
			case LANDSCAPE:
				drawRectangle((((paddedBounds.width >> 1) + (imageWidth >> 1))-((y*imageWidth/350)+height*imageWidth/350))+3
					,(((paddedBounds.height >> 1) - (imageHeight >> 1))+paddedBounds.y)+(x*imageHeight/250),
					height*imageWidth/350,
					width*imageHeight/250);
				break;
			case PORTRAIT:
				drawRectangle((5 + (paddedBounds.width >> 1) - (imageWidth >> 1))+(x*imageWidth/250),
					((paddedBounds.height >> 1) - (imageHeight >> 1))+(y*imageHeight/350),
					width*imageWidth/250,height*imageHeight/350);
				break;
		}
	}else{
		switch(_orientation) {
			case LANDSCAPE:
				drawRectangle(((paddedBounds.width >> 1) - (imageWidth >> 1))+(x*imageWidth/250)+paddedBounds.x,
					((paddedBounds.height >> 1) - (imageHeight >> 1))+(y*imageHeight/350)+paddedBounds.y,
					width*imageWidth/250,height*imageHeight/350);
				break;
			case PORTRAIT:
				drawRectangle((((paddedBounds.width >> 1) + (imageWidth >> 1))-((y*imageWidth/350)+height*imageWidth/350))+3
					,(((paddedBounds.height >> 1) - (imageHeight >> 1))+getPosition().y)+(x*imageHeight/250),
					height*imageWidth/350,width*imageHeight/250);
				break;
		}
	}
	//refreshing = true;
	Gfx_updateScreen();*/

}

void MobImage::refreshWidget(bool drawStats) {
	this->drawWidget(drawStats);
}

void MobImage::drawWidget() {
	this->drawWidget(true);
}

void MobImage::drawWidget(bool drawStats) {
	if (resource) {
		Gfx_drawImage(resource, (paddedBounds.width >> 1) - (imageWidth >> 1),
			(paddedBounds.height >> 1) - (imageHeight >> 1));
		if (hasNote) {
			Gfx_drawImage(RES_CHANGE_STAR, (paddedBounds.width >> 1) - (imageWidth >> 1),
				(paddedBounds.height >> 1) - (imageHeight >> 1));
		}
		if (statAdded) {
			//selectStat(_x - 5,_y,_width,_height,_red,_green,_blue,_orientation);
		}
	}

	if (!refreshing) {
		if (drawStats)
			this->drawStats();
	}

	if (drawRect) {
		Gfx_setColor(_red, _green, _blue);
		if(portrait==1){
			switch(_orientation) {
				case LANDSCAPE:
					drawRectangle((((paddedBounds.width >> 1) + (imageWidth >> 1))-((_y*imageWidth/350)+_height*imageWidth/350))-2
						,(((paddedBounds.height >> 1) - (imageHeight >> 1)))+(_x*imageHeight/250),
						_height*imageWidth/350,
						_width*imageHeight/250);
					break;
				case PORTRAIT:
					drawRectangle((5 + (paddedBounds.width >> 1) - (imageWidth >> 1))+(_x*imageWidth/250),
						((paddedBounds.height >> 1) - (imageHeight >> 1))+(_y*imageHeight/350),
						_width*imageWidth/250,_height*imageHeight/350);
					break;
			}
		}else{
			switch(_orientation) {
				case LANDSCAPE:
					drawRectangle(((paddedBounds.width >> 1) - (imageWidth >> 1))+(_x*imageWidth/250)+paddedBounds.x,
						((paddedBounds.height >> 1) - (imageHeight >> 1))+(_y*imageHeight/350)+paddedBounds.y,
						_width*imageWidth/250,_height*imageHeight/350);
					break;
				case PORTRAIT:
					drawRectangle((((paddedBounds.width >> 1) + (imageWidth >> 1))-((_y*imageWidth/350)+_height*imageWidth/350))+3
						,(((paddedBounds.height >> 1) - (imageHeight >> 1))+getPosition().y)+(_x*imageHeight/250),
						_height*imageWidth/350,_width*imageHeight/250);
					break;
			}
		}
	}
	refreshing = false;
}

void MobImage::drawStats() {
	if (stats.size() > 0) {
		bool portrait = (imageHeight>imageWidth);
		int diffX = (paddedBounds.width - imageWidth)/2, diffY = (paddedBounds.height - imageHeight)/2;
		for (int i = 0; i < labels.size(); i++) {
			labels[i]->setParent(NULL);
		}

		for (int i = 0; i < stats.size(); i++) {
			if (stats[i].front == front) {
				if (portrait) {
					labels[i]->setPosition(((int)(stats[i].x * ((double)imageWidth/250))) + diffX, ((int)(stats[i].y * ((double)imageHeight/350))) + diffY);
					labels[i]->setWidth((int)(stats[i].width * ((double)imageWidth/250)));
					labels[i]->setHeight((int)(stats[i].height * ((double)imageHeight/350)));
				}
				else {
					labels[i]->setPosition(((int)((350 - (stats[i].y + stats[i].height)) * ((double)imageWidth/350))) + diffX,
							((int)((stats[i].x) * ((double)imageHeight/250))) + diffY);
					labels[i]->setWidth((int)(stats[i].height * ((double)imageWidth/350)));
					labels[i]->setHeight((int)(stats[i].width * ((double)imageHeight/250)));
				}
				labels[i]->setParent(this);
				labels[i]->draw(true);
			}
		}
	}
}

void MobImage::addStat(stat stat) {
	stats.add(stat);

	MobLabel *temp = new MobLabel(0, 0, 0, 0, NULL, (imageHeight>imageWidth)?TRANS_NONE:TRANS_ROT90);
	temp->setDrawBackground(false);
	temp->setMultiLine(true);
	temp->setCaption(stat.display);
	temp->setFont(Util::getMobFontGrey());

	labels.add(temp);
}

void MobImage::clearStats() {
	for (int i = 0; i < labels.size(); i++) {
		if (labels[i]->getParent() != NULL) {
			labels[i]->setParent(NULL);
		}
		delete labels[i];
		labels[i] = NULL;
	}
	labels.clear();
	stats.clear();
}

void MobImage::flip() {
	front = !front;
}

void MobImage::setFront(bool f) {
	front = f;
}
