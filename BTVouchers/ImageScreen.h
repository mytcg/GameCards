#ifndef _IMAGESCREEN_H_
#define _IMAGESCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/ListBox.h>
#include <MAUI/Image.h>

#include "Card.h"
#include "Feed.h"
#include "MAHeaders.h"
#include "ImageCache.h"

using namespace MAUI;

/**
 *  The screen class used for demonstrating how to
 *  use MAUI::Image widgets.
 */
class ImageScreen : public Screen {
public:
	ImageScreen(Screen *previous, MAHandle image, Feed *feed, bool flip, Card *card);
	~ImageScreen();
	void keyPressEvent(int keyCode);
	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);

private:
	Screen *previous, *next;
	MAHandle img;
	Card *card;
	Feed *feed;
	Image *imge;
	Layout *mainLayout;
	ListBox *listBox;
	int height;
	bool flip;
	bool left, right, list;
};

#endif	//_IMAGESCREEN_H_
