#ifndef _TUTORIALSCREEN_H_
#define _TUTORIALSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Image.h>
#include <MAUI/ListBox.h>
#include "../utils/Card.h"
#include "../utils/Feed.h"
#include "../MAHeaders.h"
#include "../utils/ImageCache.h"
#include "../utils/XmlConnection.h"
#include "../UI/Widgets/MobImage.h"
#include "MainScreen.h"

using namespace MAUI;

struct tutItem {
	MAHandle image;
	String description;
	int key;
};

class TutorialScreen : public MainScreen {
public:
	TutorialScreen(MainScreen *previous, Vector<String> tutimages);
	~TutorialScreen();
	void keyPressEvent(int keyCode);

	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);
	String getIdFromImageString(String url);
private:
	Widget* currentSelectedKey;

	ListBox *midList;
	void clearListBox();
	MAPoint2d pointPressed, pointReleased;
	ImageCache *imageCache;
	Image *leftArrow, *rightArrow;
	Widget *softKeys;
	bool left, right, panLeft, panRight;
	int itemCount, index, height, currentKeyPosition;

	Vector<MobImage*> tutImages;

	void scrollImage(int move = 1); //1 will move 1 image to the right, -1 will move 1 to the left
};

#endif	//_TUTORIALSCREEN_H_
