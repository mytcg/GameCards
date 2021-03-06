#ifndef _FORGOTPASSWORDSCREEN_H_
#define _FORGOTPASSWORDSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/ListBox.h>
#include <MAUI/Layout.h>

#include "../utils/Feed.h"
#include "../utils/XmlConnection.h"
#include "../utils/Card.h"
#include "../UI/Native/NativeEditBox.h"
#include "../UI/KineticListBox.h"
#include "MainScreen.h"

using namespace MAUI;
using namespace MAUtil;

class ForgotPasswordScreen : public MainScreen, WidgetListener, private XCListener, Mtx::XmlListener, private HttpConnectionListener {
public:
	ForgotPasswordScreen(Feed *feed, MainScreen *previous);
	~ForgotPasswordScreen();
	void keyPressEvent(int keyCode);
	void selectionChanged(Widget *widget, bool selected);
	void show();
	void clearListBox();
	void hide();

	void pointerPressEvent(MAPoint2d point);
	void pointerMoveEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
	void locateItem(MAPoint2d point);

private:
	NativeEditBox *editBoxEmail;
	Widget* currentSelectedKey;
	HttpConnection mHttp;

	String error_msg, result, parentTag;
	bool left, right, mid, error;
	int moved, currentKeyPosition;

	bool isBusy;

	void resetPassword();

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

#endif	//_FORGOTPASSWORDSCREEN_H_
