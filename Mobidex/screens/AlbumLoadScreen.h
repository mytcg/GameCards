#ifndef _ALBUMLOADSCREEN_H_
#define _ALBUMLOADSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <maprofile.h>

#include "../utils/Feed.h"
#include "../utils/XmlConnection.h"
#include "../UI/KineticListBox.h"

using namespace MAUI;
using namespace MAUtil;

class AlbumLoadScreen : public Screen, WidgetListener, private XCListener, Mtx::XmlListener, private HttpConnectionListener {
public:
	AlbumLoadScreen(Feed *feed, Albums *album = NULL);
	~AlbumLoadScreen();
	void keyPressEvent(int keyCode);
	void selectionChanged(Widget *widget, bool selected);
	void show();
	void hide();
	int getCount();
	void Add(const char* id, const char* name);
	String *Retrieve(int id);
	void drawList();

	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);

	void refresh();
private:

	Screen *next;

	HttpConnection mHttp;
	XmlConnection xmlConn;

	Vector<String> path;
	Vector<Widget*> tempWidgets;
	String parentTag, notedate;
	String temp,temp1,error_msg,hasCards,updated;
	int size, i, moved, first;
	bool list, left, right, mid, empt, hasConnection, collapsed, shown;

	Layout *mainLayout;
	KineticListBox *listBox;
	Label *notice, *label, *noteLabel;

	Feed *feed;
	Albums *album;

	void loadDemo();
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
	void loadCategory();
	void cleanup();
};

#endif	//_ALBUMLOADSCREEN_H_*/
