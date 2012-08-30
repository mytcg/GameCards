#ifndef _VIEWACHISSCREEN_H_
#define _VIEWACHISSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>

#include "../utils/Feed.h"
#include "../utils/XmlConnection.h"
#include "../UI/Widgets/MobImage.h"
#include "../utils/Achievement.h"
#include "MainScreen.h"
#include "../utils/ImageCache.h"

using namespace MAUI;
using namespace MAUtil;

class ViewAchisScreen : public MainScreen, WidgetListener, private XCListener, Mtx::XmlListener, private HttpConnectionListener {
public:
	ViewAchisScreen(MainScreen *previous, Feed *feed);
	~ViewAchisScreen();
	void keyPressEvent(int keyCode);
	void selectionChanged(Widget *widget, bool selected);
	void show();
	void hide();
	void drawList();

	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);

private:
	Widget* currentSelectedKey;

	HttpConnection mHttp;
	ImageCache *mImageCache;

	String parentTag;
	String name,description,error_msg,incompleteImage,updated,completeImage,dateCompleted, strTarget, strProgress;
	int size, i, moved, xStart, currentKeyPosition, progress, target;
	bool list, left, right, mid,listLeft, listRight, empt, hasConnection, shown;

	ListBox *midListBox;
	Image *leftArrow, *rightArrow;

	int selectedList, itemsPerList;
	Vector<ListBox *> achiLists;
	Vector<Achievement *> achievements;
	Achievement *tempAchi;
	void switchList(int nextOrPrev);

	void httpFinished(MAUtil::HttpConnection*, int);
	void connReadFinished(Connection*, int);
	void xcConnError(int code);
	void mtxEncoding(const char*);
	void mtxTagStart(const char*, int);
	void mtxTagAttr(const char*, const char*);
	void mtxTagData(const char*, int);
	void mtxTagEnd(const char*, int);
	void mtxParseError(int);
	void mtxEmptyTagEnd();
	void mtxTagStartEnd();

	void clearListBox();

	String getIdFromImageString(String imageUrl);
};

#endif	//_VIEWACHISSCREEN_H_*/
