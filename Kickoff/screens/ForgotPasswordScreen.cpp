#include <mastdlib.h>

#include "ForgotPasswordScreen.h"
#include "../utils/Util.h"
#include "../UI/Button.h"

ForgotPasswordScreen::ForgotPasswordScreen(Feed *feed, MainScreen *previous) : mHttp(this) {
	lprintfln("ForgotPasswordScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	this->previous = previous;
	this->feed = feed;
	moved = 0;
	isBusy = false;
	next = NULL;
	currentSelectedKey = NULL;
	currentKeyPosition = -1;

	result = "";
	error_msg = "";
	parentTag = "";

	mainLayout = Util::createMainLayout("Reset", "Back", "", true);

	kinListBox = (KineticListBox*) mainLayout->getChildren()[0]->getChildren()[2];
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];
	notice->setMultiLine(true);

	label = new Label(0,0, scrWidth-PADDING*2, DEFAULT_SMALL_LABEL_HEIGHT, NULL, "To reset your password, enter your email address", 0, Util::getDefaultFont());
	label->setAutoSizeY(true);
	label->setMultiLine(true);
	label->setDrawBackground(false);
	kinListBox->add(label);

	label = Util::createEditLabel("");
	editBoxEmail = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "",L"Email");
	editBoxEmail->setDrawBackground(false);
	label->addWidgetListener(this);
	kinListBox->add(label);

	if (feed->getUnsuccessful() != "Success") {
		label->setCaption(feed->getUnsuccessful());
	}
	this->setMain(mainLayout);
	kinListBox->setSelectedIndex(1);
}

ForgotPasswordScreen::~ForgotPasswordScreen() {
	lprintfln("~ForgotPasswordScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	clearListBox();
	kinListBox->clear();
	delete mainLayout;
	mainLayout = NULL;
	if (next != NULL) {
		delete next;
		feed->remHttp();
		next = NULL;
	}
	result = "";
	error_msg = "";
	parentTag = "";
}
void ForgotPasswordScreen::clearListBox() {
	Vector<Widget*> tempWidgets;
	for (int i = 0; i < kinListBox->getChildren().size(); i++) {
		tempWidgets.add(kinListBox->getChildren()[i]);
	}
	kinListBox->clear();
	kinListBox->getChildren().clear();

	for (int j = 0; j < tempWidgets.size(); j++) {
		delete tempWidgets[j];
		tempWidgets[j] = NULL;
	}
	tempWidgets.clear();
}

void ForgotPasswordScreen::selectionChanged(Widget *widget, bool selected) {
	if(selected) {
		widget->getChildren()[0]->setSelected(true);
	} else {
		widget->getChildren()[0]->setSelected(false);
	}
}
void ForgotPasswordScreen::pointerPressEvent(MAPoint2d point)
{
    locateItem(point);
}

void ForgotPasswordScreen::pointerMoveEvent(MAPoint2d point)
{
    locateItem(point);
    moved++;
}

void ForgotPasswordScreen::pointerReleaseEvent(MAPoint2d point)
{
	if (moved <= 8) {
		if (right) {
			keyPressEvent(MAK_SOFTRIGHT);
		} else if (left) {
			keyPressEvent(MAK_SOFTLEFT);
		} else if (mid) {
			keyPressEvent(MAK_FIRE);
		}
	}
	else {
		moved = 0;
	}
}

void ForgotPasswordScreen::locateItem(MAPoint2d point)
{
	left = false;
	right = false;
	mid = false;

    Point p;
    p.set(point.x, point.y);
    for(int i = 0; i < (this->getMain()->getChildren()[1]->getChildren()).size(); i++)
	{
		if(this->getMain()->getChildren()[1]->getChildren()[i]->contains(p))
		{
			if (i == 0) {
				left = true;
			} else if (i == 1) {
				mid = true;
			} else if (i == 2) {
				right = true;
			}
			return;
		}
	}
}
void ForgotPasswordScreen::show() {
	kinListBox->getChildren()[kinListBox->getSelectedIndex()]->setSelected(true);
	Screen::show();
}

void ForgotPasswordScreen::hide() {
    kinListBox->getChildren()[kinListBox->getSelectedIndex()]->setSelected(false);
	Screen::hide();
}

void ForgotPasswordScreen::resetPassword() {
	if (editBoxEmail->getCaption().length() != 0) {
		int urlLength = 40 + URLSIZE + editBoxEmail->getCaption().length();
		char *url = new char[urlLength+1];
		memset(url,'\0',urlLength+1);
		sprintf(url, "%s?resetpassword=%s", URL, editBoxEmail->getCaption().c_str());
		lprintfln(url);
		if(mHttp.isOpen()){
			mHttp.close();
		}
		mHttp = HttpConnection(this);
		int res = mHttp.create(url, HTTP_GET);
		if(res < 0) {
			notice->setCaption("Unable to connect, try again later...");
		} else {
			notice->setCaption("Resetting password...");
			feed->addHttp();
			mHttp.finish();
		}
		delete [] url;
		url = NULL;
	} else {
		notice->setCaption("Please enter a valid code.");
		isBusy=false;
	}
}

void ForgotPasswordScreen::keyPressEvent(int keyCode) {
	error = false;
	//int index = kinListBox->getSelectedIndex();
	Widget *currentSoftKeys = mainLayout->getChildren()[mainLayout->getChildren().size() - 1];
	switch(keyCode) {
		case MAK_BACK:
		case MAK_SOFTRIGHT:
			editBoxEmail->disableListener();
			previous->show();
			break;
		case MAK_FIRE:
			if(currentSoftKeys->getChildren()[0]->isSelected()){
				keyPressEvent(MAK_SOFTLEFT);
				break;
			}else if(currentSoftKeys->getChildren()[2]->isSelected()){
				keyPressEvent(MAK_SOFTRIGHT);
				break;
			}
		case MAK_SOFTLEFT:
			if (!isBusy) {
				isBusy = true;
				resetPassword();
			}
			break;
		case MAK_UP:
			if(currentSelectedKey!=NULL){
				currentSelectedKey->setSelected(false);
				currentSelectedKey = NULL;
				currentKeyPosition = -1;
				kinListBox->getChildren()[kinListBox->getChildren().size()-1]->setSelected(true);
			}
			break;
		case MAK_DOWN:
			if(currentSelectedKey==NULL){
				kinListBox->getChildren()[kinListBox->getChildren().size()-1]->setSelected(false);
				for(int i = 0; i < currentSoftKeys->getChildren().size();i++){
					if(((Button *)currentSoftKeys->getChildren()[i])->isSelectable()){
						currentKeyPosition=i;
						currentSelectedKey= currentSoftKeys->getChildren()[i];
						currentSelectedKey->setSelected(true);
						break;
					}
				}
			}
			break;
		case MAK_LEFT:
			if(currentSelectedKey!=NULL){
				if(currentKeyPosition > 0){
					currentKeyPosition = currentKeyPosition - 1;
					for(int i = currentKeyPosition; i >= 0;i--){
						if(((Button *)currentSoftKeys->getChildren()[i])->isSelectable()){
							currentSelectedKey->setSelected(false);
							currentKeyPosition=i;
							currentSelectedKey= currentSoftKeys->getChildren()[i];
							currentSelectedKey->setSelected(true);
							break;
						}
					}
				}
			}
			break;
		case MAK_RIGHT:
			if(currentSelectedKey!=NULL){
				if(currentKeyPosition+1 < currentSelectedKey->getParent()->getChildren().size()){
					currentKeyPosition = currentKeyPosition + 1;
					for(int i = currentKeyPosition; i < currentSoftKeys->getChildren().size();i++){
						if(((Button *)currentSoftKeys->getChildren()[i])->isSelectable()){
							currentSelectedKey->setSelected(false);
							currentKeyPosition=i;
							currentSelectedKey= currentSoftKeys->getChildren()[i];
							currentSelectedKey->setSelected(true);
							break;
						}
					}
				}
			}
			break;
	}
}

void ForgotPasswordScreen::httpFinished(MAUtil::HttpConnection* http, int res) {
	error_msg = "";
	result = "";
	if (res == 200) {
		xmlConn = XmlConnection::XmlConnection();
		xmlConn.parse(http, this, this);
	} else {
		mHttp.close();
		feed->remHttp();
		notice->setCaption("Unable to connect. Please try again later.");
		isBusy = false;
	}
}

void ForgotPasswordScreen::connReadFinished(Connection* conn, int result) {
}

void ForgotPasswordScreen::xcConnError(int code) {
	feed->remHttp();
}

void ForgotPasswordScreen::mtxEncoding(const char* ) {
}

void ForgotPasswordScreen::mtxTagStart(const char* name, int len) {
	parentTag = name;
}

void ForgotPasswordScreen::mtxTagAttr(const char* attrName, const char* attrValue) {
}

void ForgotPasswordScreen::mtxTagData(const char* data, int len) {
	if(!strcmp(parentTag.c_str(), "result")) {
		result = data;
	} else if(!strcmp(parentTag.c_str(), "error")) {
		error_msg = data;
	}
}

void ForgotPasswordScreen::mtxTagEnd(const char* name, int len) {
	if(!strcmp(name, "result")) {
		notice->setCaption(result);
		isBusy = false;
	} else if(!strcmp(name, "error")) {
		notice->setCaption(error_msg.c_str());
	}
}

void ForgotPasswordScreen::mtxParseError(int) {
}

void ForgotPasswordScreen::mtxEmptyTagEnd() {
}

void ForgotPasswordScreen::mtxTagStartEnd() {
}
