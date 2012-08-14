#ifndef _ACHIDETAILSSCREEN_H_
#define _ACHIDETAILSSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>
#include <MAUI/Layout.h>

#include "../UI/Widgets/MobImage.h"
#include "../utils/Achievement.h"
#include "MainScreen.h"
#include "../utils/ImageCache.h"

using namespace MAUI;
using namespace MAUtil;

class AchiDetailsScreen : public MainScreen, WidgetListener {
public:
	AchiDetailsScreen(MainScreen *previous, Achievement *achi);
	~AchiDetailsScreen();
	void keyPressEvent(int keyCode);
	void selectionChanged(Widget *widget, bool selected);

	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);

private:
	//ui components
	Label *title, *description, *progress;
	ListBox *badgeList;

	Widget* currentSelectedKey;

	ImageCache *mImageCache;

	int moved, currentKeyPosition;
	bool list, left, right;

	Achievement *achi;

	String getIdFromImageString(String imageUrl);
};

#endif	//_ACHIDETAILSSCREEN_H_*/
