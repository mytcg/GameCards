#ifndef _EDITDECKSCREEN_H_
#define _EDITDECKSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Label.h>

#include "../utils/XmlConnection.h"
#include "../utils/Feed.h"
#include "../utils/Card.h"
#include "../utils/Stat.h"
#include "../utils/ImageCache.h"
#include "../UI/KineticListBox.h"
#include "../UI/Widgets/MobImage.h"
#include "MainScreen.h"

using namespace MAUI;
using namespace MAUtil;

class EditDeckScreen : public MainScreen, WidgetListener, private XCListener, Mtx::XmlListener, private HttpConnectionListener {
public:
	EditDeckScreen(MainScreen *previous, Feed *feed, String deckId, String type="1", String active="1");
	~EditDeckScreen();
	void keyPressEvent(int keyCode);
	void selectionChanged(Widget *widget, bool selected);
	void removeCard();
	void show();
	void hide();

	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);

	typedef Map<String, Card*> StringCardMap;

	enum albumTypes {ST_LIST, ST_CONFIRM_DELETE};

	void refresh();
	void setNewDeck(bool n);
private:
	ListBox *midListBox;

	HttpConnection mHttp;
	ImageCache *mImageCache;
	Widget* currentSelectedKey;

	String parentTag, statDesc, statIVal, statDisplay, note, deckId, deckCategory, type, active, hasCards;
	String id,usercardid,description,slotdescription,cardcategory_id,categoryaddon_id,quantity, thumburl, fronturl, frontflipurl, backurl, backflipurl, filename,error_msg, rate, rarity, ranking, value, positionid, position, points, updated;
	int statTop, statLeft, statWidth, statHeight, statFrontOrBack, statRed, statGreen, statBlue;
	int size, i, moved, listSizes, screenType, currentKeyPosition, cardsPerList, xStart;
	bool list, left, right, emp, hasConnection, busy, deleting, newdeck, removed, listLeft, listRight;

	Card *card;
	Vector<Card *> cards;
	Vector<Stat*> stats;
	Stat *stat;
	Image *leftArrow, *rightArrow;
	MobImage *tempImage;

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

	void clearCards();
	void clearListBox();

	void deleteDeck();

	void drawList();
	void drawConfirm();
	
	int selectedList;
	Vector<ListBox *> cardLists;

	void switchList(int nextOrPrev);
};

#endif	//_EDITDECKSCREEN_H_*/
