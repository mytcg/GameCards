#ifndef _ALBUMVIEWSCREEN_H_
#define _ALBUMVIEWSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <maprofile.h>

#include "../utils/XmlConnection.h"
#include "../utils/Feed.h"
#include "../utils/Card.h"
#include "../utils/Stat.h"
#include "../utils/ImageCache.h"
#include "../UI/KineticListBox.h"
#include "../UI/Widgets/MobImage.h"

using namespace MAUI;
using namespace MAUtil;

class AlbumViewScreen : public Screen, WidgetListener, private XCListener, Mtx::XmlListener, private HttpConnectionListener {
public:
	AlbumViewScreen(Screen *previous, Feed *feed, String category, int albumType=AT_NORMAL, Card *card = NULL, String deckId = "");
	~AlbumViewScreen();
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

	void loadFile();
	void loadImages(const char *text);

	typedef Map<String, Card*> StringCardMap;

	void refresh();

	void setDeckId(String deckId);

	int albumType;

	enum albumTypes {AT_NORMAL, AT_NEW_CARDS, AT_BUY, AT_FREE, AT_PRODUCT};
private:
	Screen *next, *previous;
	Label *notice, *label;
	KineticListBox *listBox;
	Layout *mainLayout;

	HttpConnection mHttp;
	XmlConnection xmlConn;
	ImageCache *mImageCache;

	String parentTag, statDesc, statIVal, statDisplay, note, category, deckId;
	String id,description,quantity, thumburl, fronturl, frontflipurl, backurl, backflipurl, filename,error_msg, rate, rarity, ranking, value, updated;
	int statTop, statLeft, statWidth, statHeight, statFrontOrBack, statRed, statGreen, statBlue;
	int size, i, moved, listSizes;
	bool list, left, right, emp, hasConnection, busy;

	Feed *feed;
	Card *card, *newCard;
	StringCardMap tmp, cards;
	StringCardMap::Iterator cardExists;
	Vector<String> index;
	Vector<Card *> deleted;
	Vector<Stat*> stats;
	Stat *stat;
	MobImage *tempImage;

	String getAll();
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

	//void clearFeedLayouts();
	void clearCardMap();
	void deleteCards();
	void clearListBox();

	void addCard(String cardId);
};

#endif	//_ALBUMVIEWSCREEN_H_*/
