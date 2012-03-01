#ifndef _SEARCHSCREEN_H_
#define _SEARCHSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/EditBox.h>
#include <MAUI/ListBox.h>
#include <MAUI/Layout.h>

#include "../utils/Feed.h"
#include "../utils/XmlConnection.h"
#include "../utils/Card.h"
#include "../UI/Native/NativeEditBox.h"
#include "../UI/KineticListBox.h"

using namespace MAUI;
using namespace MAUtil;

class SearchScreen : public Screen, WidgetListener, private XCListener, Mtx::XmlListener, private HttpConnectionListener {
public:
	SearchScreen(Feed *feed, Screen *previous);
	~SearchScreen();
	void keyPressEvent(int keyCode);
	void selectionChanged(Widget *widget, bool selected);
	void show();
	void hide();
	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);
private:
	Layout *mainLayout;
	ListBox *listBox;
	Label *label, *notice;
	NativeEditBox *editBoxSearch;
	Vector<Widget*> tempWidgets;

	HttpConnection mHttp;
	XmlConnection xmlConn;

	String parentTag, cardText, statDesc, statIVal, statDisplay, note, rate, value, orientation;
	String error_msg, id,description,quantity, thumburl, fronturl, backurl, updated;
	int statTop, statLeft, statWidth, statHeight, statFrontOrBack, statRed, statGreen, statBlue;
	bool left, right, mid, error, fresh;
	int moved;

	Feed *feed;

	Screen *next, *prev;
	bool isBusy, isActive;

	Map<String, Card*> cards;
	Vector<Stat*> stats;
	Stat *stat;

	void doSearch();
	void clearCardMap();

	void httpFinished(MAUtil::HttpConnection*, int);
	void connReadFinished(Connection*, int);
	void cleanup();
	void xcConnError(int code);
	void mtxEncoding(const char*);
	void mtxTagStart(const char*, int);
	void mtxTagAttr(const char*, const char*);
	void mtxTagData(const char*, int);
	void mtxTagEnd(const char*, int);
	void mtxParseError(int);
	void mtxEmptyTagEnd();
	void mtxTagStartEnd();
};

#endif	//_SEARCHSCREEN_H_
