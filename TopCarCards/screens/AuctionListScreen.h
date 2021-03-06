#ifndef _AUCTIONLISTSCREEN_H_
#define _AUCTIONLISTSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Label.h>

#include "../utils/XmlConnection.h"
#include "../utils/Feed.h"
#include "../utils/Auction.h"
#include "../utils/ImageCache.h"
#include "../UI/KineticListBox.h"
#include "../UI/Widgets/MobImage.h"
#include "MainScreen.h"

using namespace MAUI;
using namespace MAUtil;

class AuctionListScreen : public MainScreen, WidgetListener, private XCListener, Mtx::XmlListener, private HttpConnectionListener {
public:
	AuctionListScreen(MainScreen *previous, Feed *feed, int screenType, String categoryId = NULL);
	~AuctionListScreen();
	void keyPressEvent(int keyCode);
	void selectionChanged(Widget *widget, bool selected);
	void drawList();

	void refresh();

	void updateAuctions();

	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);

	typedef Vector<Auction*> AuctionVector;

	enum screenTypes {ST_CATEGORY, ST_USER};
private:
	ImageCache *mImageCache;
	MobImage *tempImage;
	Widget* currentSelectedKey;

	HttpConnection mHttp;

	String parentTag, cardText;
	String cardId, description, thumburl, fronturl, frontflipurl, backurl, backflipurl, categoryId, error_msg, openingBid, price, userCardId, auctionCardId, username, buyNowPrice, endDate, lastBidUser;
	int moved, screenType, currentKeyPosition;
	bool list, left, right, emp, shouldUpdateAuction, busy;

	AuctionVector auctions;
	AuctionVector deleted;
	AuctionVector::iterator auctionIter;
	Card *card;
	Auction *auction;

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
	String getTime(String enddate);
	void clearAuctions();
	void deleteAuctions();
};

#endif	//_AUCTIONLISTSCREEN_H_*/
