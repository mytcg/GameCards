#ifndef _DECKLISTSCREEN_H_
#define _DECKLISTSCREEN_H_

#include <ma.h>
#include <MAUI/ListBox.h>
#include <MAUI/Label.h>
#include <MAUI/Layout.h>
#include <MAUI/Screen.h>
#include <maprofile.h>

#include "../utils/XmlConnection.h"
#include "../utils/Feed.h"
#include "../utils/Album.h"
#include "../utils/Card.h"
#include "../UI/KineticListBox.h"

using namespace MAUI;
using namespace MAUtil;

class DeckListScreen : public Screen, WidgetListener, private XCListener, Mtx::XmlListener, private HttpConnectionListener {
public:
	DeckListScreen(Screen *previous, Feed *feed, Card *card);
	~DeckListScreen();
	void keyPressEvent(int keyCode);
	void selectionChanged(Widget *widget, bool selected);

	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);

	enum screenPhase {SP_LIST = 0,
			SP_ADDING, SP_FINISHED};
private:
	Screen *previous;
	Screen *next;

	Feed *feed;
	Layout *layout;
	KineticListBox* kinListBox;
	Label *lbl, *notice;
	Album *album;
	Vector<Album*> albums;

	Card *card;

	bool list, left, right, selecting;

	HttpConnection mHttp;
	XmlConnection xmlConn;

	String parentTag;
	String deckId, description, result;
	int moved, phase;

	void clearListBox();
	void clearAlbums();
	void drawList();

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
};

#endif