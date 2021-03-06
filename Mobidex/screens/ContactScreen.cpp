#include <mastdlib.h>
#include <IX_PIM.h>
#include <mawstring.h>
#include <wchar.h>

#include "ContactScreen.h"
#include "../utils/Util.h"
#include "AlbumViewScreen.h"
#include "TradeFriendDetailScreen.h"


ContactScreen::ContactScreen(MainScreen *previous) {
	this->previous = previous;
	lprintfln("ContactScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	moved = 0;
	index = -1;
	isBusy = false;

	layout = Util::createMainLayout("Search", "Back", "", false);

	listBox = (KineticListBox*) layout->getChildren()[0]->getChildren()[2];

	notice = (Label*) layout->getChildren()[0]->getChildren()[1];

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "Enter contact name", 0, Util::getDefaultFont());
	listBox->add(label);

	label = Util::createEditLabel("");
	editBoxSearch = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, 48-PADDING*2, 64, MA_TB_TYPE_ANY, label, "",L"Search term:");
#if defined(MA_PROF_SUPPORT_STYLUS)

#else
	editBoxSearch->setInputMode(NativeEditBox::IM_NUMBERS);
#endif
	editBoxSearch->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	contactListBox = new KineticListBox(0, 0, scrWidth, listBox->getHeight() - editBoxSearch->getParent()->getHeight() - 24,
		listBox, KineticListBox::LBO_VERTICAL, KineticListBox::LBA_LINEAR, false);

	this->setMain(layout);

	listBox->setSelectedIndex(1);
	editBoxSearch->setSelected(true);

	//load all the contacts into a list
	Contact *temp;
	char *name, *number;
	String strName, strNumber;

	MAHandle mContactsListHandle = maPimListOpen(MA_PIM_CONTACTS);
	MAHandle pimItemHandle = maPimListNext(mContactsListHandle);

	MA_PIM_ARGS args;
	args.item = pimItemHandle;
	args.bufSize = 512;
	char *mBuffer;
	mBuffer = new char[512];
	args.buf = mBuffer;

	while (pimItemHandle > 0) {
		strName = "";
		strNumber = "";

		args.item = pimItemHandle;

		int countFields = maPimItemCount(pimItemHandle);
		if (countFields >= 0)
		{
			args.field = MA_PIM_FIELD_CONTACT_NAME;

			// Get value from name field at position 0.
			maPimItemGetValue(&args, 0);

			// there can be up to 8 parts of a name
			for (int i = 0; i < 8; i++)
			{
				const wchar* contactNameValue = Util::getWCharArrayFromBuf(args.buf, i);
				if (contactNameValue && *contactNameValue != 0)
				{
					name = new char[wcslen(contactNameValue)];
					memset(name, '\0', wcslen(contactNameValue));
					sprintf(name, "%S", contactNameValue);

					if (strName.length() > 0) {
						strName.append(", ", 2);
					}
					strName.append(name, strlen(name));

					delete name;
				}
			}

			//printf("name: %s", strName.c_str());

			args.field = MA_PIM_FIELD_CONTACT_TEL;

			// Get the number of values.
			int countValues =
				maPimItemFieldCount(pimItemHandle, MA_PIM_FIELD_CONTACT_TEL);

			for (int i = 0; i < countValues; i++)
			{
				int result = maPimItemGetValue(&args, i);

				if (result >= 0) {
					number = new char[wcslen((wchar*) args.buf)];
					memset(number, '\0', wcslen((wchar*) args.buf));
					sprintf(number, "%S", (wchar*) args.buf);
				}

				strNumber = number;
				temp = new Contact(strName, strNumber);
				allContacts.add(temp);

				delete number;
				//printf("number: %s", strNumber.c_str());
			}
		}

		maPimItemClose(pimItemHandle);
		pimItemHandle = maPimListNext(mContactsListHandle);
	}
	maPimListClose(mContactsListHandle);
}

ContactScreen::~ContactScreen() {
	contacts.clear();
	for (int i = 0; i < allContacts.size(); i++) {
		delete allContacts[i];
		allContacts[i] = NULL;
	}
	allContacts.clear();

	delete layout;
}

void ContactScreen::selectionChanged(Widget *widget, bool selected) {
	if (widget->getParent() == listBox) {
		if(selected) {
			widget->getChildren()[0]->setSelected(true);
		} else {
			widget->getChildren()[0]->setSelected(false);
		}
	}
}
void ContactScreen::pointerPressEvent(MAPoint2d point)
{
    locateItem(point);
}

void ContactScreen::pointerMoveEvent(MAPoint2d point)
{
    locateItem(point);
    moved++;
}

void ContactScreen::pointerReleaseEvent(MAPoint2d point)
{
	if (moved <= 8) {
		if (right) {
			keyPressEvent(MAK_SOFTRIGHT);
		} else if (left) {
			keyPressEvent(MAK_SOFTLEFT);
		} else if (mid || list) {
			keyPressEvent(MAK_FIRE);
		}
	}
	else {
		moved = 0;
	}
}

void ContactScreen::locateItem(MAPoint2d point)
{
	left = false;
	right = false;
	mid = false;
	list = false;

    Point p;
    p.set(point.x, point.y);
    if (editBoxSearch->contains(p)) {
		editBoxSearch->enableListener();
		editBoxSearch->setSelected(true);
    }
    else {
		editBoxSearch->disableListener();
		editBoxSearch->setSelected(false);
	}
    for(int i = 0; i < contactListBox->getChildren().size() && contactListBox->contains(p); i++)
	{
		if(contactListBox->getChildren()[i]->contains(p))
		{
			index = i;
			list = true;
		}
	}
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

void ContactScreen::doSearch() {
	contacts.clear();

	String searchString = editBoxSearch->getCaption();

	int found = -1;
	for (int i = 0; i < allContacts.size(); i++) {
		found = Util::findIgnoreCase(allContacts[i]->getName(), searchString);


		if (found >= 0) {
			contacts.add(allContacts[i]);
		}
	}

	clearListBox();
	String display = "";

	for(int j = 0; j < contacts.size(); j++) {
		display = contacts[j]->getName() + "\n" + contacts[j]->getNumber();
		label = Util::createSubLabel(display.c_str());
		label->setPaddingBottom(5);
		label->setPaddingLeft(5);
		label->addWidgetListener(this);
		contactListBox->add(label);
	}

	index = -1;
	editBoxSearch->setSelected(true);
	if (contacts.size() > 0) {
		Util::updateSoftKeyLayout("Search", "Back", "Select", layout);
	}
	else {
		Util::updateSoftKeyLayout("Search", "Back", "", layout);
	}
}

void ContactScreen::clearListBox() {
	for (int i = 0; i < contactListBox->getChildren().size(); i++) {
		tempWidgets.add(contactListBox->getChildren()[i]);
	}
	contactListBox->clear();
	contactListBox->getChildren().clear();
	contactListBox->selectNextItem();

	for (int j = 0; j < tempWidgets.size(); j++) {
		delete tempWidgets[j];
		tempWidgets[j] = NULL;
	}
	tempWidgets.clear();
}

void ContactScreen::keyPressEvent(int keyCode) {
	switch(keyCode) {
		case MAK_SOFTRIGHT:
		case MAK_BACK:
			editBoxSearch->setSelected(false);
			editBoxSearch->disableListener();
			previous->show();
			break;
		case MAK_SOFTLEFT:
			doSearch();
			break;
		case MAK_FIRE:
			if (index >= 0) {
				((TradeFriendDetailScreen*)previous)->contactSelected(contacts[contactListBox->getSelectedIndex()]->getNumber());
			}
			break;
		case MAK_UP:
			if (editBoxSearch->isSelected() && contacts.size() > 0) {
				editBoxSearch->setSelected(false);
				listBox->setSelectedIndex(2);
				contactListBox->setSelectedIndex(contactListBox->getChildren().size() - 1);
				index = contactListBox->getSelectedIndex();
			}
			else if (contactListBox->getSelectedIndex() == 0 && contacts.size() > 0) {
				listBox->setSelectedIndex(1);
				contactListBox->getChildren()[contactListBox->getSelectedIndex()]->setSelected(false);
				editBoxSearch->setSelected(true);
				index = -1;
			}
			else if (contacts.size() > 0) {
				contactListBox->selectPreviousItem(true);
				index = contactListBox->getSelectedIndex();
			}
			break;
		case MAK_DOWN:
			if (editBoxSearch->isSelected() && contacts.size() > 0) {
				editBoxSearch->setSelected(false);
				listBox->setSelectedIndex(2);
				contactListBox->setSelectedIndex(0);
				index = contactListBox->getSelectedIndex();
			}
			else if (contactListBox->getSelectedIndex() == (contacts.size() - 1)) {
				listBox->setSelectedIndex(1);
				contactListBox->getChildren()[contactListBox->getSelectedIndex()]->setSelected(false);
				editBoxSearch->setSelected(true);
				index = -1;
			}
			else if (contacts.size() > 0) {
				contactListBox->selectNextItem(true);
				index = contactListBox->getSelectedIndex();
			}
			break;
	}
}
