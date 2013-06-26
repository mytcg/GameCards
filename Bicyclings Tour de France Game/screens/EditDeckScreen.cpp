#include <conprint.h>
#include <mastdlib.h>

#include "DeckListScreen.h"
#include "EditDeckScreen.h"
#include "AlbumLoadScreen.h"
#include "AlbumViewScreen.h"
#include "../utils/Util.h"
#include "ImageScreen.h"
#include "CompareScreen.h"
#include "OptionsScreen.h"
#include "ShopDetailsScreen.h"
#include "../UI/Button.h"

EditDeckScreen::EditDeckScreen(MainScreen *previous, Feed *feed, String deckId, String type, String active) : mHttp(this), deckId(deckId), type(type), active(active) {
	busy = true;
	emp = true;
	this->previous = previous;
	this->feed = feed;
	deleting = false;
	removed = false;
	newdeck = false;
	currentSelectedKey = NULL;
	leftArrow = NULL;
	rightArrow = NULL;
	currentKeyPosition = -1;
	next = NULL;

	lprintfln("EditDeckScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());

	id = "";
	usercardid = "";
	description = "";
	slotdescription="";
	cardcategory_id="";
	categoryaddon_id="";
	quantity = "";
	thumburl = "";
	fronturl = "";
	frontflipurl = "";
	backurl = "";
	backflipurl = "";
	rate = "";
	ranking = "";
	rarity = "";
	value = "";
	positionid = "";
	position = "";
	points = "";
	error_msg = "";
	updated = "";
	statDisplay = "";
	note = "";
	statDesc = "";
	statIVal = "";
	deckCategory = "";
	hasCards = "";
	closingDate = "";
	int port = 1;
	if(portrait == false){
		port = 2;
	}

	mainLayout = Util::createMainLayout("", "Back" , "");

	listBox = (ListBox*) mainLayout->getChildren()[0]->getChildren()[2];
	listBox->setWrapping(true);
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];
	notice->setCaption("Getting card list...");

	mImageCache = new ImageCache();
	int urlLength = 88 + URLSIZE + strlen("deck_id") + deckId.length() + Util::intlen(scrHeight) + Util::intlen(scrWidth) + type.length();
	char *url = new char[urlLength+1];
	memset(url,'\0',urlLength+1);
	sprintf(url, "%s?getcardsindeck=1&deck_id=%s&height=%d&portrait=%d&width=%d&jpg=1&type=%s", URL,
			deckId.c_str(), Util::getMaxImageHeight(), port, Util::getMaxImageWidth(), type.c_str());
	lprintfln("%s", url);
	if(mHttp.isOpen()){
		mHttp.close();
	}
	mHttp = HttpConnection(this);
	int res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		busy = false;
		hasConnection = false;
		notice->setCaption("");
	} else {
		hasConnection = true;
		mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
		mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
		feed->addHttp();
		mHttp.finish();

	}
	delete [] url;
	url = NULL;

	this->setMain(mainLayout);
	moved=0;

	orig = this;
}

void EditDeckScreen::refresh() {
	clearListBox();
	clearCards();
	hasCards = "";
	closingDate = "";
	notice->setCaption("Refreshing card list...");
	
	int port = 1;
	if(portrait == false){
		port = 2;
	}
	int urlLength = 88 + URLSIZE + strlen("deck_id") + deckId.length() + Util::intlen(scrHeight) + Util::intlen(scrWidth) + type.length();
	char *url = new char[urlLength+1];
	memset(url,'\0',urlLength+1);
	sprintf(url, "%s?getcardsindeck=1&deck_id=%s&height=%d&portrait=%d&width=%d&jpg=1&type=%s", URL,
			deckId.c_str(), Util::getMaxImageHeight(), port, Util::getMaxImageWidth(), type.c_str());
	lprintfln("%s", url);
	if(mHttp.isOpen()){
		mHttp.close();
	}
	mHttp = HttpConnection(this);
	int res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		busy = false;
		hasConnection = false;
		notice->setCaption("");
	} else {
		hasConnection = true;
		mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
		mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
		feed->addHttp();
		mHttp.finish();

	}
	delete [] url;
	url = NULL;
}

void EditDeckScreen::deleteDeck() {
	deleting = true;

	notice->setCaption("Deleting deck...");

	int urlLength = 71 + URLSIZE + strlen("deck_id") + deckId.length() + type.length();
	char *url = new char[urlLength+1];
	memset(url,'\0',urlLength+1);
	sprintf(url, "%s?deletedeck=1&deck_id=%s&type=%s", URL,	deckId.c_str(),type.c_str());
	lprintfln("%s", url);
	if(mHttp.isOpen()){
		mHttp.close();
	}
	mHttp = HttpConnection(this);
	int res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		busy = false;
		hasConnection = false;
		notice->setCaption("");
	} else {
		hasConnection = true;
		mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
		mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
		feed->addHttp();
		mHttp.finish();

	}
	delete [] url;
	url = NULL;
}

void EditDeckScreen::removeCard() {
	int cardIndex = (cardsPerList * selectedList) + cardLists[selectedList]->getSelectedIndex();
	if ((!strcmp(type.c_str(), "1")||!strcmp(type.c_str(), "2"))) {
			cardIndex -= 1;
	}//-1 for the "delete deck" option
	if (cards.size() < 10 && (!strcmp(type.c_str(), "1")||!strcmp(type.c_str(), "2"))) {
		cardIndex -= 1; //if there are less than 10 cards, there is also the "add card" option
	}
	int urlLength = 71 + URLSIZE + strlen("deck_id") + deckId.length() + strlen("card_id") +
		cards[cardIndex]->getId().length()+type.length();
	char *url = new char[urlLength+1];
	memset(url,'\0',urlLength+1);
	sprintf(url, "%s?removefromdeck=1&deck_id=%s&card_id=%s&type=%s", URL,
			deckId.c_str(), cards[cardIndex]->getId().c_str(), type.c_str());
	lprintfln("%s", url);
	if(mHttp.isOpen()){
		mHttp.close();
	}
	mHttp = HttpConnection(this);
	int res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		busy = false;
		hasConnection = false;
		notice->setCaption("");
	} else {
		hasConnection = true;
		mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
		mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
		feed->addHttp();
		mHttp.finish();

	}
	delete [] url;
	url = NULL;

	if((!strcmp(type.c_str(), "1")||!strcmp(type.c_str(), "2"))){
		delete cards[cardIndex];
		cards[cardIndex] = NULL;
		cards.remove(cardIndex);

		drawList();
	}else{
		removed = true;
	}
}

void EditDeckScreen::pointerPressEvent(MAPoint2d point) {
	xStart = point.x;
    locateItem(point);
}

void EditDeckScreen::pointerMoveEvent(MAPoint2d point) {
    locateItem(point);
    moved++;
}

void EditDeckScreen::pointerReleaseEvent(MAPoint2d point) {
	if (!emp && (list || listLeft || listRight)) {
		int xEnd = point.x;
		int distance = abs(xEnd - xStart);

		if (distance >= (scrWidth * 0.4)) {
			moved=0;
			switchList(xEnd>xStart?-1:1);
			return;
		}
	}
	if (moved <= 8) {
		if (right) {
			keyPressEvent(MAK_SOFTRIGHT);
		} else if (left) {
			keyPressEvent(MAK_SOFTLEFT);
		} else if (list) {
			keyPressEvent(MAK_FIRE);
		} else if (listLeft) {
			keyPressEvent(MAK_LEFT);
		} else if (listRight) {
			keyPressEvent(MAK_RIGHT);
		}
	}
	moved=0;
}

void EditDeckScreen::locateItem(MAPoint2d point) {
	list = false;
	left = false;
	right = false;
	listLeft = false;
	listRight = false;
	Point p;
	p.set(point.x, point.y);

	if (leftArrow != NULL && leftArrow->contains(p)) {
		listLeft = true;
		return;
	}
	else if (rightArrow != NULL && rightArrow->contains(p)) {
		listRight = true;
		return;
	}
	for(int i = 0; selectedList < cardLists.size() &&
		i < (cardLists[selectedList]->getChildren()).size(); i++)
	{
		if(cardLists[selectedList]->getChildren()[i]->contains(p))
		{
			list = true;
			cardLists[selectedList]->setSelectedIndex(i);
		}
	}
	for(int i = 0; i < (this->getMain()->getChildren()[1]->getChildren()).size(); i++)
	{
		if(this->getMain()->getChildren()[1]->getChildren()[i]->contains(p))
		{
			if (i == 0) {
				left = true;
				moved=0;
			} else if (i == 2) {
				right = true;
				moved=0;
			}
			return;
		}
	}
}

void EditDeckScreen::switchList(int nextOrPrev) {
	int currentIndex = cardLists[selectedList]->getSelectedIndex();

	if ((selectedList + nextOrPrev >= 0) && (selectedList + nextOrPrev < cardLists.size())) {
		selectedList += nextOrPrev;
	}
	else if (selectedList + nextOrPrev < 0) {
		selectedList = cardLists.size() - 1;
	}
	else if (selectedList + nextOrPrev >= cardLists.size()) {
		selectedList = 0;
	}
	midListBox->clear();
	midListBox->add(cardLists[selectedList]);

	currentIndex = currentIndex>=cardLists[selectedList]->getChildren().size()?cardLists[selectedList]->getChildren().size()-1:currentIndex;

	cardLists[selectedList]->setSelectedIndex(currentIndex);
	cardLists[selectedList]->getChildren()[currentIndex]->setSelected(true);

	int capLength = 6 + Util::intlen((selectedList + 1)) + Util::intlen(cardLists.size());
	char *cap = new char[capLength+1];
	memset(cap,'\0',capLength+1);
	sprintf(cap, "Page %d/%d", (selectedList + 1), cardLists.size());
	((Label*)this->getMain()->getChildren()[1]->getChildren()[1])->setCaption(cap);

	currentIndex = 0;
}

void EditDeckScreen::clearListBox() {
	Vector<Widget*> tempWidgets;

	if (!emp) {
		midListBox->clear();
		for (int i = 0; i < cardLists.size(); i++) {
			tempWidgets.add(cardLists[i]);
		}
		cardLists.clear();
		for (int i = 0; i < listBox->getChildren().size(); i++) {
			tempWidgets.add(listBox->getChildren()[i]);
		}
		listBox->clear();
		listBox->getChildren().clear();
	}
	else {
		for (int i = 0; i < listBox->getChildren().size(); i++) {
			tempWidgets.add(listBox->getChildren()[i]);
		}
		listBox->clear();
		listBox->getChildren().clear();
	}

	for (int j = 0; j < tempWidgets.size(); j++) {
		delete tempWidgets[j];
		tempWidgets[j] = NULL;
	}
	tempWidgets.clear();

}

void EditDeckScreen::drawList() {

	screenType = ST_LIST;

	Layout *feedlayout;

	int ind = listBox->getSelectedIndex();
	if (ind < 0) {
		ind = 0;
	}

	int items = cards.size();
	if (!strcmp(type.c_str(), "1")) {
		items++;
	}


	//clearListBox();
	//we need a layout to have arrow images on the sides of the list
	cardsPerList = listBox->getHeight() / ALBUM_ITEM_HEIGHT; //74 is the default card display item height
	if(cardsPerList == 0){
		cardsPerList = 1;
	}
	Layout *listLayout;
	//check if we need more than 1 page
	if (cardsPerList < items) {
		listLayout = new Layout(0, 0, listBox->getWidth(), listBox->getHeight(), listBox, 3, 1);
		listLayout->setDrawBackground(false);
		listLayout->setVerticalAlignment(Layout::VA_CENTER);

		leftArrow = new Image(0, 0, ARROW_WIDTH, listLayout->getHeight(), listLayout, false, false, RES_LEFT_ARROW);
		leftArrow->setDrawBackground(false);

		midListBox = new ListBox(0, 0, listLayout->getWidth() - (ARROW_WIDTH*2) - (PADDING*2), listLayout->getHeight(), listLayout, ListBox::LBO_VERTICAL);
		midListBox->setDrawBackground(false);

		rightArrow = new Image(0, 0, ARROW_WIDTH, listLayout->getHeight(), listLayout, false, false, RES_RIGHT_ARROW);
		rightArrow->setDrawBackground(false);
	} else {
		listLayout = new Layout(0, 0, listBox->getWidth(), listBox->getHeight(), listBox, 1, 1);
		listLayout->setDrawBackground(false);
		listLayout->setVerticalAlignment(Layout::VA_CENTER);

		leftArrow = new Image(0, 0, ARROW_WIDTH, listLayout->getHeight(), NULL, false, false, RES_LEFT_ARROW);
		leftArrow->setDrawBackground(false);

		midListBox = new ListBox(0, 0, listLayout->getWidth() - (PADDING*2), listLayout->getHeight(), listLayout, ListBox::LBO_VERTICAL);
		midListBox->setDrawBackground(false);

		rightArrow = new Image(0, 0, ARROW_WIDTH, listLayout->getHeight(), NULL, false, false, RES_RIGHT_ARROW);
		rightArrow->setDrawBackground(false);
	}
	int currentList = -1;
	ListBox *tempList = NULL;
	int j = 0;

	int i = 0;
	do {
		//gotta make the tempList for the cards
		if (j % cardsPerList == 0) {
			tempList = new ListBox(0, 0, midListBox->getWidth(), midListBox->getHeight(), NULL);
			tempList->setDrawBackground(false);
			tempList->setOrientation(ListBox::LBO_VERTICAL);
			currentList++;
			cardLists.add(tempList);

			if (i == 0 && cards.size() < 10 && (!strcmp(type.c_str(), "1")||!strcmp(type.c_str(), "2"))) {
				feedlayout = new Layout(0, 0, tempList->getWidth()-(PADDING*2), 48, tempList, 3, 1);
				feedlayout->setSkin(Util::getSkinList());
				feedlayout->setDrawBackground(true);
				feedlayout->addWidgetListener(this);

				label = new Label(0, 0, 0, 0, NULL, "", 0, Util::getDefaultFont());
				feedlayout->add(label);

				label = new Label(0, 0, feedlayout->getWidth(), DEFAULT_LABEL_HEIGHT, feedlayout);
				label->setCaption("Add Card");
				label->setHorizontalAlignment(Label::HA_CENTER);
				label->setVerticalAlignment(Label::VA_CENTER);
				label->setSkin(Util::getSkinList());
				label->setDrawBackground(false);

				j++;
			}
			if (i == 0 && !strcmp(type.c_str(), "1")||!strcmp(type.c_str(), "2")) {
				feedlayout = new Layout(0, 0, tempList->getWidth()-(PADDING*2), 48, tempList, 3, 1);
				feedlayout->setSkin(Util::getSkinList());
				feedlayout->setDrawBackground(true);
				feedlayout->addWidgetListener(this);

				label = new Label(0, 0, 0, 0, NULL, "", 0, Util::getDefaultFont());
				feedlayout->add(label);

				label = new Label(0, 0, feedlayout->getWidth(), DEFAULT_LABEL_HEIGHT, feedlayout);
				label->setCaption("Delete Deck");
				label->setHorizontalAlignment(Label::HA_CENTER);
				label->setVerticalAlignment(Label::VA_CENTER);
				label->setSkin(Util::getSkinList());
				label->setDrawBackground(false);

				j++;
			}
		}
		if (i < cards.size()) {
			String cardText = "";
			if (!strcmp(type.c_str(), "1") || !strcmp(type.c_str(), "2") || !strcmp(type.c_str(), "3")) {
				cardText += cards[i]->getText();
				cardText += " (";
				cardText += cards[i]->getQuantity();
				cardText += ")\n";
				cardText += cards[i]->getRarity();
				cardText += "\nRating: ";
				cardText += cards[i]->getRanking();
			} else if(!strcmp(type.c_str(), "4")) {
				if(strcmp(cards[i]->getText().c_str(),"")){
					cardText += cards[i]->getText()+"\n";
				}
				cardText += "Position: ";
				cardText += cards[i]->getPosition();
				cardText += "\nPoints: ";
				cardText += cards[i]->getPoints();
			}

			feedlayout = new Layout(0, 0, tempList->getWidth()-(PADDING*2), ALBUM_ITEM_HEIGHT + ((midListBox->getHeight() % THUMB_HEIGHT) / cardsPerList), tempList, 3, 1);
			feedlayout->setSkin(Util::getSkinAlbum());
			feedlayout->setDrawBackground(true);
			feedlayout->addWidgetListener(this);
			feedlayout->setVerticalAlignment(Layout::VA_CENTER);

			if (strcmp(cards[i]->getQuantity().c_str(), "0") != 0) {
				//if the card is in the deck, the image must be downloaded
				tempImage = new MobImage(0, 0, 56, 64, feedlayout, false, false, Util::loadImageFromResource(RES_LOADINGTHUMB));
				tempImage->setHasNote(cards[i]->getNote().length()>0);
				Util::retrieveThumb(tempImage, cards[i], mImageCache);
			}
			else {
				//we use the blank image for empty positions
				tempImage = new MobImage(0, 0, 56, 64, feedlayout, false, false, Util::loadImageFromResource(RES_MISSINGTHUMB));
			}

			label = new Label(0,0, scrWidth-86, ALBUM_ITEM_HEIGHT, feedlayout, cardText, 0, Util::getDefaultFont());
			cardText = "";

			tempImage->setDrawBackground(false);
			label->setDrawBackground(false);
			label->setVerticalAlignment(Label::VA_CENTER);
			label->setAutoSizeY();
			label->setAutoSizeX();
			label->setMultiLine();

			j++;
		}
		i++;
	}
	while(i < cards.size());

	//for(int i = 0; i < cards.size(); i++) {}

	if (items >= 1) {
		emp = false;
		int listIndex = ind / cardsPerList;
		int listItem = ind % cardsPerList;
		selectedList = listIndex;
		midListBox->add(cardLists[listIndex]);
		cardLists[listIndex]->setSelectedIndex(listItem);
		cardLists[listIndex]->getChildren()[cardLists[listIndex]->getSelectedIndex()]->setSelected(true);
	} else {
		emp = true;
		midListBox->add(Util::createSubLabel("Empty"));
		midListBox->setSelectedIndex(0);
	}
	int capLength = 6 + Util::intlen((selectedList + 1)) + Util::intlen(items);
	char *cap = new char[capLength+1];
	memset(cap,'\0',capLength+1);
	sprintf(cap, "Page %d/%d", (selectedList + 1), cardLists.size());
	((Label*)this->getMain()->getChildren()[1]->getChildren()[1])->setCaption(cap);
	if (currentSelectedKey!=NULL) {
		currentSelectedKey->setSelected(false);
		currentSelectedKey = NULL;
		currentKeyPosition = -1;
	}
}

void EditDeckScreen::drawConfirm() {
	screenType = ST_CONFIRM_DELETE;

	clearListBox();

	label = new Label(0, 0, listBox->getWidth() - 10, 0, listBox, "Are you sure you want to delete this deck?", 0, Util::getDefaultFont());
	label->setAutoSizeY();
	label->setMultiLine(true);
	label->setDrawBackground(false);
	label->setPaddingLeft(5);
	label->setPaddingTop(5);
	label->setPaddingRight(5);

	currentSelectedKey = NULL;
	currentKeyPosition = -1;
	Util::updateSoftKeyLayout("Confirm", "Back", "", mainLayout);
}

EditDeckScreen::~EditDeckScreen() {
	lprintfln("~EditDeckScreen::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	delete mainLayout;
	mainLayout = NULL;
	if(next != NULL){
		delete next;
		feed->remHttp();
		next = NULL;
	}
	delete mImageCache;
	mImageCache = NULL;

	clearCards();

	stats.clear();
	parentTag="";
	statDesc="";
	statIVal="";
	statDisplay="";
	note="";
	deckId="";
	id="";
	usercardid="";
	description="";
	slotdescription="";
	cardcategory_id="";
	categoryaddon_id="";
	quantity="";
	thumburl="";
	fronturl="";
	frontflipurl="";
	backurl="";
	backflipurl="";
	filename="";
	error_msg="";
	rate="";
	rarity="";
	ranking="";
	value="";
	positionid = "";
	position = "";
	points = "";
	updated="";
	deckCategory="";
	active = "";
	type = "";
	hasCards = "";
}

void EditDeckScreen::selectionChanged(Widget *widget, bool selected) {
	switch (screenType) {
		case ST_LIST:
			if(selected) {
				((Label *)widget->getChildren()[1])->setFont(Util::getDefaultSelected());
			} else {
				((Label *)widget->getChildren()[1])->setFont(Util::getDefaultFont());
			}

			//if ((kinListBox->getSelectedIndex() == 1 && cards.size() < 10) ||
			//		(kinListBox->getSelectedIndex() == 0)) {
			//	Util::updateSoftKeyLayout("", "Back", "", mainLayout);
			//}
			//else {
			//	Util::updateSoftKeyLayout("Remove", "Back", "", mainLayout);
			//}
			break;
	}
}

void EditDeckScreen::show() {
	Screen::show();
}

void EditDeckScreen::hide() {
	Screen::hide();
}

void EditDeckScreen::keyPressEvent(int keyCode) {
	String all = "";
	int ind = (cardsPerList * selectedList) + cardLists[selectedList]->getSelectedIndex();
	if (!strcmp(type.c_str(), "1")||!strcmp(type.c_str(), "2")) {
		if (cards.size() < 10) {
			ind -= 1; //if there are less than 10 cards, there is also the "add card" option
		}
		ind -= 1; //for the "delete deck" option
	}

	Widget *currentSoftKeys = mainLayout->getChildren()[mainLayout->getChildren().size() - 1];
	switch (screenType) {
		case ST_LIST:
			switch(keyCode) {
				case MAK_UP:
					if(currentSelectedKey!=NULL){
						currentSelectedKey->setSelected(false);
						currentSelectedKey = NULL;
						currentKeyPosition = -1;
						if (!busy) {
							cardLists[selectedList]->getChildren()[cardLists[selectedList]->getChildren().size()-1]->setSelected(true);
						}
					}
					else if (!busy) {
						if (cardLists[selectedList]->getSelectedIndex() == 0) {
							cardLists[selectedList]->setSelectedIndex(cardLists[selectedList]->getChildren().size()-1);
						} else {
							cardLists[selectedList]->selectPreviousItem();
						}
					}
					break;
				case MAK_DOWN:
					if (cardLists[selectedList]->getSelectedIndex() < (cardLists[selectedList]->getChildren().size() - 1) && !busy) {
						cardLists[selectedList]->setSelectedIndex(cardLists[selectedList]->getSelectedIndex()+1);
					} else if(currentSelectedKey==NULL) {
						if (!busy) {
							cardLists[selectedList]->getChildren()[cardLists[selectedList]->getSelectedIndex()]->setSelected(false);
						}
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
				case MAK_BACK:
				case MAK_SOFTRIGHT:
					if(newdeck){
						((DeckListScreen*)previous)->refresh();
					}
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
					//lprintfln("type %s active %s",type.c_str(),active.c_str());
					if(!strcmp(type.c_str(), "1")||!strcmp(type.c_str(), "2")){
						if (cardLists[selectedList]->getSelectedIndex() == 0 && selectedList == 0 && cards.size() < 10) {
							if (next != NULL) {
								delete next;
								feed->remHttp();
								next = NULL;
							}
							//if the category has cards, we need to go straight to AlbumView, instead of album load
							if (hasCards=="true") {
								next = new AlbumViewScreen(this, feed, deckCategory, AlbumViewScreen::AT_DECK, false, NULL, deckId,"",cards[ind]->getPositionId());
							}
							else {
								next = new AlbumLoadScreen(this, feed, AlbumLoadScreen::ST_DECK, NULL, false, NULL, deckCategory);
								((AlbumLoadScreen*)next)->setDeckId(deckId);
							}
							next->show();
						}
						else if ((selectedList == 0 && cardLists[selectedList]->getSelectedIndex() == 0 && cards.size() == 10) ||
								(selectedList == 0 && cardLists[selectedList]->getSelectedIndex() == 1 && cards.size() < 10)) {
							drawConfirm();
						}else if((selectedList == 0 && cardLists[selectedList]->getSelectedIndex() > 0 && cards.size() == 10) ||
								(selectedList == 0 && cardLists[selectedList]->getSelectedIndex() > 1 && cards.size() < 10)){
							if (next != NULL) {
								delete next;
								feed->remHttp();
								next = NULL;
							}
							if(!strcmp(type.c_str(), "1")){
								next = new ImageScreen(this, Util::loadImageFromResource(portrait?RES_LOADING1:RES_LOADING_FLIP1), feed, false, cards[ind],ImageScreen::ST_DECK_REMOVE);
								next->show();
							}else if(!strcmp(type.c_str(), "2")){
								next = new ImageScreen(this, Util::loadImageFromResource(portrait?RES_LOADING1:RES_LOADING_FLIP1), feed, false, cards[ind],ImageScreen::ST_DECK_OPTIONS);
								next->show();
							}
						}
					} else if(!strcmp(type.c_str(), "3")&&(!strcmp(cards[ind]->getText().c_str(),""))){
						if (next != NULL) {
							delete next;
							feed->remHttp();
							next = NULL;
						}
						next = new AlbumViewScreen(this, feed, cards[ind]->getCategoryId(), AlbumViewScreen::AT_DECK_ADDON, false, cards[ind], deckId);
						next->show();
					} else if(!strcmp(type.c_str(), "3")&&strcmp(cards[ind]->getText().c_str(),"")){
						if (next != NULL) {
							delete next;
							feed->remHttp();
							next = NULL;
						}
						next = new ImageScreen(this, Util::loadImageFromResource(portrait?RES_LOADING1:RES_LOADING_FLIP1), feed, false, cards[ind],ImageScreen::ST_DECK_ADDON, false, card);
						next->show();
					} else if(!strcmp(type.c_str(), "4")&&!strcmp(active.c_str(), "1")){
						if (next != NULL) {
							delete next;
							feed->remHttp();
							next = NULL;
						}
						//if the category has cards, we need to go straight to AlbumView, instead of album load
						if (hasCards=="true") {
							next = new AlbumViewScreen(this, feed, deckCategory, AlbumViewScreen::AT_DECK, false, NULL, deckId,"",cards[ind]->getPositionId());
						}
						else {
							next = new AlbumLoadScreen(this, feed, AlbumLoadScreen::ST_DECK, NULL, false, NULL, deckCategory);
							((AlbumLoadScreen*)next)->setDeckId(deckId);
							((AlbumLoadScreen*)next)->setPositionId(cards[ind]->getPositionId());
						}
						next->show();
					}
					break;
				case MAK_SOFTLEFT:
					if ((cardLists[selectedList]->getSelectedIndex() == 0 && selectedList == 0) && cards.size() < 10 && !strcmp(type.c_str(), "1")) {
						if (next != NULL) {
							delete next;
							feed->remHttp();
							next = NULL;
						}
						//if the category has cards, we need to go straight to AlbumView, instead of album load
						if (hasCards=="true") {
							next = new AlbumViewScreen(this, feed, deckCategory, AlbumViewScreen::AT_DECK, false, NULL, deckId,"",cards[ind]->getPositionId());
						}
						else {
							next = new AlbumLoadScreen(this, feed, AlbumLoadScreen::ST_DECK, NULL, false, NULL, deckCategory);
							((AlbumLoadScreen*)next)->setDeckId(deckId);
						}
						next->show();
					}
					else if (selectedList == 0 && ((cardLists[selectedList]->getSelectedIndex() == 0 && cards.size() == 10) ||
							(cardLists[selectedList]->getSelectedIndex() == 1 && cards.size() < 10))) {
						drawConfirm();
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
					} else if (!emp) {
						switchList(-1);
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
					} else if (!emp) {
						switchList(1);
					}
					break;
			}
			break;
		case ST_CONFIRM_DELETE:
			switch(keyCode) {
				case MAK_FIRE:
					if(currentSoftKeys->getChildren()[0]->isSelected()){
						keyPressEvent(MAK_SOFTLEFT);
					}else if(currentSoftKeys->getChildren()[2]->isSelected()){
						keyPressEvent(MAK_SOFTRIGHT);
					}
					break;
				case MAK_UP:
					if(currentSelectedKey!=NULL){
						currentSelectedKey->setSelected(false);
						currentSelectedKey = NULL;
						currentKeyPosition = -1;
						cardLists[selectedList]->getChildren()[cardLists[selectedList]->getChildren().size()-1]->setSelected(true);
					}
					else if (ind == 0) {
						cardLists[selectedList]->setSelectedIndex((cardLists[selectedList]->getChildren().size())-1);
					} else {
						cardLists[selectedList]->selectPreviousItem();
					}
					break;
				case MAK_DOWN:
					if (cardLists[selectedList]->getSelectedIndex()+1 < cardLists[selectedList]->getChildren().size() ) {
						cardLists[selectedList]->setSelectedIndex(ind+1);
					} else if(currentSelectedKey==NULL) {
						cardLists[selectedList]->getChildren()[cardLists[selectedList]->getSelectedIndex()]->setSelected(false);
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
				case MAK_BACK:
				case MAK_SOFTRIGHT:
					if (!busy) {
						//busy = true;
						clearListBox();
						Util::updateSoftKeyLayout("", "Back", "", mainLayout);
						drawList();
					}
					break;
				case MAK_SOFTLEFT:
					if (!busy) {
						busy = true;
						deleteDeck();
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
			break;
	}
}

void EditDeckScreen::httpFinished(MAUtil::HttpConnection* http, int result) {
	error_msg = "";
	if (result == 200) {
		xmlConn = XmlConnection::XmlConnection();
		xmlConn.parse(http, this, this);
	} else {
		mHttp.close();
		//notice->setCaption("");
		busy = false;
		feed->remHttp();
	}
}

void EditDeckScreen::connReadFinished(Connection* conn, int result) {}

void EditDeckScreen::xcConnError(int code) {
	feed->remHttp();
	if (removed){
		removed=false;
		refresh();
	}
}

void EditDeckScreen::mtxEncoding(const char* ) {
}

void EditDeckScreen::mtxTagStart(const char* name, int len) {
	parentTag = name;
	if (!strcmp(name, "cardsincategory")) {
		clearCards();
	}
}

void EditDeckScreen::mtxTagAttr(const char* attrName, const char* attrValue) {
	if(!strcmp(parentTag.c_str(), "stat")) {
		if(!strcmp(attrName, "desc")) {
			statDesc = attrValue;
		}else if(!strcmp(attrName, "ival")) {
			statIVal = attrValue;
		}else if(!strcmp(attrName, "top")) {
			statTop = atoi(attrValue);
		}else if(!strcmp(attrName, "left")) {
			statLeft = atoi(attrValue);
		}else if(!strcmp(attrName, "width")) {
			statWidth = atoi(attrValue);
		}else if(!strcmp(attrName, "height")) {
			statHeight = atoi(attrValue);
		}else if(!strcmp(attrName, "frontorback")) {
			statFrontOrBack = atoi(attrValue);
		}else if(!strcmp(attrName, "red")) {
			statRed = atoi(attrValue);
		}else if(!strcmp(attrName, "green")) {
			statGreen = atoi(attrValue);
		}else if(!strcmp(attrName, "blue")) {
			statBlue = atoi(attrValue);
		}
	}
	else if (!strcmp(parentTag.c_str(), "deck")) {
		if(!strcmp(attrName, "closingDate")) {
			closingDate = attrValue;
		}
	}
}

void EditDeckScreen::mtxTagData(const char* data, int len) {
	if(!strcmp(parentTag.c_str(), "cardid")) {
		id = data;
	} else if(!strcmp(parentTag.c_str(), "usercardid")) {
		usercardid = data;
	} else if(!strcmp(parentTag.c_str(), "description")) {
		description = data;
	} else if(!strcmp(parentTag.c_str(), "slotdescription")) {
		slotdescription = data;
	} else if(!strcmp(parentTag.c_str(), "cardcategory_id")) {
		cardcategory_id = data;
	} else if(!strcmp(parentTag.c_str(), "categoryaddon_id")) {
		categoryaddon_id = data;
	} else if(!strcmp(parentTag.c_str(), "quantity")) {
		quantity = data;
	} else if(!strcmp(parentTag.c_str(), "thumburl")) {
		thumburl = data;
	} else if(!strcmp(parentTag.c_str(), "fronturl")) {
		fronturl = data;
	} else if(!strcmp(parentTag.c_str(), "frontflipurl")) {
		frontflipurl = data;
	} else if(!strcmp(parentTag.c_str(), "backurl")) {
		backurl = data;
	} else if(!strcmp(parentTag.c_str(), "backflipurl")) {
		backflipurl = data;
	} else if(!strcmp(parentTag.c_str(), "rate")) {
		rate = data;
	} else if(!strcmp(parentTag.c_str(), "ranking")) {
		ranking = data;
	} else if(!strcmp(parentTag.c_str(), "quality")) {
		rarity += data;
	} else if(!strcmp(parentTag.c_str(), "value")) {
		value = data;
	} else if(!strcmp(parentTag.c_str(), "positionid")) {
		positionid = data;
	} else if(!strcmp(parentTag.c_str(), "position")) {
		position = data;
	} else if(!strcmp(parentTag.c_str(), "points")) {
		points = data;
	} else if(!strcmp(parentTag.c_str(), "result")) {
		error_msg = data;
	} else if(!strcmp(parentTag.c_str(), "updated")) {
		updated = data;
	} else if(!strcmp(parentTag.c_str(), "stat")) {
		statDisplay = data;
	} else if(!strcmp(parentTag.c_str(), "note")) {
		note = data;
	} else if(!strcmp(parentTag.c_str(), "category_id")) {
		deckCategory = data;
	} else if(!strcmp(parentTag.c_str(), "hascards")) {
		hasCards += data;
	}
}

void EditDeckScreen::mtxTagEnd(const char* name, int len) {
	if(!strcmp(name, "card")) {
		//notice->setCaption("");
		Card *newCard = new Card();
		newCard->setPositionId(positionid.c_str());
		newCard->setPosition(position.c_str());
		newCard->setPoints(points.c_str());
		newCard->setAll((quantity+","+description+","+thumburl+","+fronturl+","+backurl+","+id+","+rate+","+value+","+note+","+ranking+","+rarity+","+frontflipurl+","+backflipurl+",").c_str());
		newCard->setStats(stats);
		newCard->setUpdated(updated == "1");
		newCard->setUserCardId(usercardid.c_str());
		newCard->setSlotDescription(slotdescription.c_str());
		newCard->setCategoryId(cardcategory_id.c_str());
		newCard->setCategoryAddonId(categoryaddon_id.c_str());
		newCard->setDeckId(deckId.c_str());
		cards.add(newCard);
		id = "";
		usercardid = "";
		description = "";
		slotdescription="";
		cardcategory_id="";
		categoryaddon_id="";
		quantity = "";
		thumburl = "";
		fronturl = "";
		backurl = "";
		rate = "";
		value = "";
		positionid = "";
		position = "";
		points = "";
		rarity = "";
		ranking = "";
		frontflipurl = "";
		backflipurl = "";
		updated = "";
		note = "";
		stats.clear();
		//delete newCard;
		//newCard = NULL;
	} else if(!strcmp(name, "stat")) {
		stat = new Stat();
		stat->setDesc(statDesc.c_str());
		stat->setDisplay(statDisplay.c_str());
		stat->setIVal(statIVal.c_str());
		stat->setTop(statTop);
		stat->setLeft(statLeft);
		stat->setWidth(statWidth);
		stat->setHeight(statHeight);
		stat->setFrontOrBack(statFrontOrBack);
		stat->setColorRed(statRed);
		stat->setColorGreen(statGreen);
		stat->setColorBlue(statBlue);
		stats.add(stat);

		statDesc = "";
		statDisplay = "";
		statIVal = "";
		stat = NULL;
		//delete stat;
	} else if(!strcmp(name, "result")) {
		//notice->setCaption(error_msg.c_str());
		statDesc="";
		statIVal="";
		statDisplay="";
		note="";
		id="";
		description="";
		slotdescription="";
		quantity="";
		thumburl="";
		fronturl="";
		frontflipurl="";
		backurl="";
		backflipurl="";
		error_msg="";
		rate="";
		rarity="";
		ranking="";
		value="";
		updated="";

		if (deleting) {
			((DeckListScreen*)previous)->refresh();
			previous->show();
		}
	} else if (!strcmp(name, "deck")) {
		drawList();
		busy = false;
		lprintfln("closingDate: %s", closingDate.c_str());
		if (closingDate.length() > 0) {
			notice->setCaption("Competition closes: " + closingDate);
		}
		else {
			notice->setCaption("");
		}
		//notice->setCaption("");
		statDesc="";
		statIVal="";
		statDisplay="";
		note="";
		id="";
		description="";
		slotdescription="";
		quantity="";
		thumburl="";
		fronturl="";
		frontflipurl="";
		backurl="";
		backflipurl="";
		error_msg="";
		rate="";
		rarity="";
		ranking="";
		value="";
		positionid = "";
		position = "";
		points = "";
		updated="";
	}
}

void EditDeckScreen::clearCards() {
	for (int i = 0; i < cards.size(); i++) {
		delete cards[i];
		cards[i] = NULL;
	}
	cards.clear();

	for (int j = 0; j < stats.size(); j++) {
		stats[j] = NULL;
	}
	stats.clear();
}

void EditDeckScreen::setNewDeck(bool nd) {
	newdeck = nd;
}

void EditDeckScreen::mtxParseError(int) {
}

void EditDeckScreen::mtxEmptyTagEnd() {
}

void EditDeckScreen::mtxTagStartEnd() {
}
