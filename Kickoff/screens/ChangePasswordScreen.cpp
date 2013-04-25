#include <mastdlib.h>

#include "ChangePasswordScreen.h"
#include "../utils/Util.h"
#include "../UI/Button.h"

ChangePasswordScreen::ChangePasswordScreen(Feed *feed, MainScreen *previous) : mHttp(this) {
	lprintfln("ChangePasswordScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	this->previous = previous;
	this->feed = feed;
	moved = 0;
	isBusy = false;
	next = NULL;
	currentSelectedKey = NULL;
	currentKeyPosition = -1;

	encrypt = "";
	setEncrypt = "";
	result = "";
	error_msg = "";
	parentTag = "";

	mainLayout = Util::createMainLayout("Change", "Back", "", true);

	kinListBox = (KineticListBox*) mainLayout->getChildren()[0]->getChildren()[2];
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];
	notice->setMultiLine(true);

	label = new Label(0,0, scrWidth-PADDING*2, DEFAULT_SMALL_LABEL_HEIGHT, NULL, "Old Password:", 0, Util::getDefaultFont());
	label->setAutoSizeY(true);
	label->setMultiLine(true);
	label->setDrawBackground(false);
	kinListBox->add(label);

	label = Util::createEditLabel("");
	editBoxOldPass = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "",L"Email");
	editBoxOldPass->setDrawBackground(false);
	label->addWidgetListener(this);
	kinListBox->add(label);

	label = new Label(0,0, scrWidth-PADDING*2, DEFAULT_SMALL_LABEL_HEIGHT, NULL, "New Password:", 0, Util::getDefaultFont());
	label->setAutoSizeY(true);
	label->setMultiLine(true);
	label->setDrawBackground(false);
	kinListBox->add(label);

	label = Util::createEditLabel("");
	editBoxNewPass = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "",L"Email");
	editBoxNewPass->setDrawBackground(false);
	label->addWidgetListener(this);
	kinListBox->add(label);

	label = new Label(0,0, scrWidth-PADDING*2, DEFAULT_SMALL_LABEL_HEIGHT, NULL, "Confirm Password:", 0, Util::getDefaultFont());
	label->setAutoSizeY(true);
	label->setMultiLine(true);
	label->setDrawBackground(false);
	kinListBox->add(label);

	label = Util::createEditLabel("");
	editBoxConfirm = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "",L"Email");
	editBoxConfirm->setDrawBackground(false);
	label->addWidgetListener(this);
	kinListBox->add(label);

	if (feed->getUnsuccessful() != "Success") {
		label->setCaption(feed->getUnsuccessful());
	}
	this->setMain(mainLayout);
	kinListBox->setSelectedIndex(1);
}

ChangePasswordScreen::~ChangePasswordScreen() {
	lprintfln("~ChangePasswordScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	clearListBox();
	kinListBox->clear();
	delete mainLayout;
	mainLayout = NULL;
	if (next != NULL) {
		delete next;
		feed->remHttp();
		next = NULL;
	}
	setEncrypt = "";
	encrypt = "";
	result = "";
	setEncrypt = "";
	error_msg = "";
	parentTag = "";
}

void ChangePasswordScreen::clearListBox() {
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

void ChangePasswordScreen::selectionChanged(Widget *widget, bool selected) {
	if(selected) {
		widget->getChildren()[0]->setSelected(true);
	} else {
		widget->getChildren()[0]->setSelected(false);
	}
}

void ChangePasswordScreen::pointerPressEvent(MAPoint2d point)
{
    locateItem(point);
}

void ChangePasswordScreen::pointerMoveEvent(MAPoint2d point)
{
    locateItem(point);
    moved++;
}

void ChangePasswordScreen::pointerReleaseEvent(MAPoint2d point)
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

void ChangePasswordScreen::locateItem(MAPoint2d point)
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

void ChangePasswordScreen::show() {
	kinListBox->getChildren()[kinListBox->getSelectedIndex()]->setSelected(true);
	Screen::show();
}

void ChangePasswordScreen::hide() {
    kinListBox->getChildren()[kinListBox->getSelectedIndex()]->setSelected(false);
    editBoxConfirm->disableListener();
	editBoxNewPass->disableListener();
	editBoxOldPass->disableListener();
	Screen::hide();
}

void ChangePasswordScreen::changePassword() {
	if (editBoxOldPass->getText().length() == 0) {
		notice->setCaption("Please enter your current password.");
		isBusy=false;
	}
	else if (editBoxNewPass->getText() != editBoxConfirm->getText()) {
		notice->setCaption("The Confirm Password field must match your newpassword.");
		isBusy=false;
	}
	else if (editBoxNewPass->getText().length() < 6) {
		notice->setCaption("Your password needs to be at least 6 characters long.");
		isBusy=false;
	}
	else {
		encrypt = Util::base64_encode(reinterpret_cast<const unsigned char*>(editBoxNewPass->getCaption().c_str()),editBoxNewPass->getCaption().length());

		int urlLength = 40 + URLSIZE + editBoxNewPass->getCaption().length() + editBoxOldPass->getCaption().length();
		char *url = new char[urlLength+1];
		memset(url,'\0',urlLength+1);
		sprintf(url, "%s?changepassword=%s&oldpass=%s", URL, editBoxNewPass->getCaption().c_str(), editBoxOldPass->getCaption().c_str());
		lprintfln(url);
		if(mHttp.isOpen()){
			mHttp.close();
		}
		mHttp = HttpConnection(this);
		int res = mHttp.create(url, HTTP_GET);
		if(res < 0) {
			notice->setCaption("Unable to connect, try again later...");
		} else {
			notice->setCaption("Changing password...");
			mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
			mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
			feed->addHttp();
			mHttp.finish();
		}
		delete [] url;
		url = NULL;
	}
}

void ChangePasswordScreen::keyPressEvent(int keyCode) {
	error = false;
	//int index = kinListBox->getSelectedIndex();
	Widget *currentSoftKeys = mainLayout->getChildren()[mainLayout->getChildren().size() - 1];
	int index = kinListBox->getSelectedIndex();
	switch(keyCode) {
		case MAK_BACK:
		case MAK_SOFTRIGHT:
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
				changePassword();
			}
			break;
		case MAK_UP:
			if(currentSelectedKey!=NULL){
				currentSelectedKey->setSelected(false);
				currentSelectedKey = NULL;
				currentKeyPosition = -1;
				kinListBox->getChildren()[kinListBox->getChildren().size()-1]->setSelected(true);
			}
			else if (index-2 > 0) {
				kinListBox->setSelectedIndex(index-2);
			}
			break;
		case MAK_DOWN:
			if (index+2 < kinListBox->getChildren().size()) {
				kinListBox->setSelectedIndex(index+2);
			}
			else if(currentSelectedKey==NULL){
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

void ChangePasswordScreen::httpFinished(MAUtil::HttpConnection* http, int res) {
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

void ChangePasswordScreen::connReadFinished(Connection* conn, int result) {
}

void ChangePasswordScreen::xcConnError(int code) {
	feed->remHttp();
}

void ChangePasswordScreen::mtxEncoding(const char* ) {
}

void ChangePasswordScreen::mtxTagStart(const char* name, int len) {
	parentTag = name;
}

void ChangePasswordScreen::mtxTagAttr(const char* attrName, const char* attrValue) {
}

void ChangePasswordScreen::mtxTagData(const char* data, int len) {
	if(!strcmp(parentTag.c_str(), "result")) {
		result = data;
	} else if(!strcmp(parentTag.c_str(), "error")) {
		error_msg = data;
	}
	else if(!strcmp(parentTag.c_str(), "setEncrypt")) {
		setEncrypt = data;
	}
}

void ChangePasswordScreen::mtxTagEnd(const char* name, int len) {
	if(!strcmp(name, "error")) {
		notice->setCaption(error_msg.c_str());
	}
	else if(!strcmp(name, "response")) {
		notice->setCaption(result);

		if (!strcmp(setEncrypt.c_str(), "true")) {

			feed->setEncrypt(encrypt.c_str());
		}

		isBusy = false;
	}
}

void ChangePasswordScreen::mtxParseError(int) {
}

void ChangePasswordScreen::mtxEmptyTagEnd() {
}

void ChangePasswordScreen::mtxTagStartEnd() {
}
