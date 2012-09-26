#ifndef MOBIMAGE_H_
#define MOBIMAGE_H_

#include <MAUtil/Moblet.h>
#include <MAUI/Widget.h>
#include <MAUI/Image.h>
#include <MAUtil/util.h>
#include <MAUtil/Vector.h>

#include "MobLabel.h"

using namespace MAUtil;
using namespace MAUI;

struct stat {
	String key;
	String display;
	int x;
	int y;
	int height;
	int width;
	int red;
	int green;
	int blue;
	bool front;
};

class MobImage: public Image {
public:
	MobImage(int x=0, int y=0, int width=0, int height=0, Widget* parent=NULL,
			bool autosizeX=false, bool autosizeY=false, MAHandle res=NULL, bool front=true);
	~MobImage();

	void setHasNote(bool n);
	void setResource(MAHandle res);
	void selectStat(int x, int y, int width, int height, int red, int green, int blue, int orientation=0);
	bool statContains(int x, int y, int width, int height, int pointX, int pointY, int orientation=0);
	void drawRectangle(int x, int y, int width, int height);
	void refreshWidget();
	bool statAdded;

	enum orientations {PORTRAIT = 0, LANDSCAPE };

	void addStat(stat stat);
	void flip();
protected:
	virtual void drawWidget();
	int _x, _y, _width, _height, _red, _green, _blue, _orientation;
	bool hasNote;

private:
	Vector<stat> stats;
	Vector<MobLabel*> labels;

	bool front;
};

#endif /* MOBIMAGE_H_ */
