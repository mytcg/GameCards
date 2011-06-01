#include <conprint.h>
#include <mastdlib.h>

#include "GamePlayScreen.h"
#include "../utils/Util.h"
#include "../utils/MAHeaders.h"
#include "ImageScreen.h"
#include "OptionsScreen.h"

//in the case of a new game, identifier is the categoryId. For an existing game, it is the gameId.
GamePlayScreen::GamePlayScreen(Screen *previous, Feed *feed, bool newGame, String identifier) : mHttp(this),
		previous(previous), feed(feed), newGame(newGame) {

	parentTag = "";
	cardText = "";
	id = "";
	description = "";
	thumburl = "";
	fronturl = "";
	backurl = "";
	error_msg = "";
	gamePlayerCardId = "";
	statType = "";
	statDescription = "";
	cardStatId = "";
	cardName = "";
	userScore = "";
	opponentScore = "";
	explanation = "";
	outcome = "";
	message = "";

	feedLayouts = NULL;
	next = NULL;

	busy = false;
	flip = false;
	loadingGame = false;

	currentSelectedStat = -1;
	flipOrSelect = 0;
	cardIndex = 0;
	yOffset = 0;
	storeHeight = 0;

	card = NULL;

	imageCache = new ImageCache();

	mainLayout = createMainLayout(back, "", "", true);
	listBox = (KineticListBox*) mainLayout->getChildren()[0]->getChildren()[2];
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];
	notice->setDrawBackground(true);

	listBox->setHeight(listBox->getHeight() - 20);

	phase = P_LOADING;

	char *url;
	if (newGame) {
		categoryId = identifier;

		notice->setCaption(initialising_game);

		//work out how long the url will be, the 2 is for the & and = symbals
		int urlLength = NEWGAME.length() + 2 + strlen(category_id) + categoryId.length();
		url = new char[urlLength];
		memset(url,'\0',urlLength);
		sprintf(url, "%s&%s=%s", NEWGAME.c_str(), category_id, categoryId.c_str());
	}
	else {
		gameId = identifier;
		loadingGame = true;
		notice->setCaption(loading_game);

		//work out how long the url will be, the 17 is for the & and = symbals, as well as hard coded vars
		int urlLength = LOADGAME.length() + 17 + strlen(game_id) + gameId.length() + intlen(scrHeight) + intlen(scrWidth);
		url = new char[urlLength];
		memset(url,'\0',urlLength);
		sprintf(url, "%s&%s=%s&height=%d&width=%d", LOADGAME.c_str(), game_id,
				gameId.c_str(), getMaxImageHeight(), scrWidth);
	}
	if(mHttp.isOpen()){
		mHttp.close();
	}
	mHttp = HttpConnection(this);
	int res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		hasConnection = false;
		notice->setCaption("Connection error.");
	} else {
		hasConnection = true;
		mHttp.setRequestHeader(auth_user, feed->getUsername().c_str());
		mHttp.setRequestHeader(auth_pw, feed->getEncrypt().c_str());
		mHttp.finish();
	}
	delete [] url;
	this->setMain(mainLayout);

	moved=0;

	orig = this;
}

void GamePlayScreen::clearListBox() {
	imageCache->clearImageCache();

	for (int i = 0; i < listBox->getChildren().size(); i++) {
		tempWidgets.add(listBox->getChildren()[i]);
	}
	listBox->clear();
	listBox->getChildren().clear();

	for (int j = 0; j < tempWidgets.size(); j++) {
		delete tempWidgets[j];
		tempWidgets[j] = NULL;
	}
	tempWidgets.clear();
}

void GamePlayScreen::drawStatList() {
	for(int i = 0; i < cardStats.size(); i++) {
		label = createSubLabel(cardStats[i]->getDesc() + " : " + cardStats[i]->getDisplay());
		label->setPaddingBottom(5);
		label->addWidgetListener(this);
		listBox->add(label);
	}

	if (cardStats.size() >= 1) {
		listBox->setSelectedIndex(0);
	} else {
		label = createSubLabel(empty);
		label->addWidgetListener(this);
		listBox->add(label);
	}
}

void GamePlayScreen::drawStatListScreen() {
	phase = P_SELECT_STAT;
	listBox->setEnabled(true);
	clearListBox();

	updateSoftKeyLayout(options, select, "", mainLayout);

	String display = "Card chosen: " + cardName + "\n";
	display.append(select_stat, strlen(select_stat));

	notice->setCaption(display);

	drawStatList();
}

void GamePlayScreen::drawResultsScreen() {
	lprintfln("explanation: %s", explanation.c_str());

	clearListBox();

	updateSoftKeyLayout(options, continuelbl, "", mainLayout);

	notice->setCaption("");
	Label *lbl = new Label(0, 0, scrWidth-(PADDING*2), 0, NULL);
	lbl->setFont(gFontBlack);
	lbl->setAutoSizeY(true);
	lbl->setMultiLine(true);

	String display = "";
	if (phase == P_FINISHED) {
		lbl->setCaption("Final score");
		display += "Scores\nYou: "+userScore+"\nOpponent: "+opponentScore;
		display += "\nResult: " + outcome;
	}
	else {
		lbl->setCaption("Turn results");

		display = explanation;
		display += "\n\nScores\nYou: "+userScore+"\nOpponent: "+opponentScore;
	}
	listBox->add(lbl);

	lbl = new Label(0, 0, scrWidth-(PADDING*2), 0, NULL);
	lbl->setFont(gFontBlack);
	lbl->setAutoSizeY(true);
	lbl->setMultiLine(true);
	lbl->setCaption(display);
	listBox->add(lbl);

	userScore = "";
	opponentScore = "";
	explanation = "";
}

void GamePlayScreen::drawCardDetailsScreen() {
	imageCache->clearImageCache();
	listBox->setEnabled(false);
	phase = P_CARD_DETAILS;

	storeHeight = mainLayout->getChildren()[0]->getChildren()[0]->getHeight();
	mainLayout->getChildren()[0]->getChildren()[0]->setHeight(0);
	listBox->setHeight(scrHeight-(mainLayout->getChildren()[1]->getHeight()));
	notice->setCaption("");
	listBox->setPosition(0, 0);

	flip = true;

	notice->setCaption("");

	clearListBox();

	updateSoftKeyLayout(back, play_card, "", mainLayout);

	int height = listBox->getHeight();

	tempImage = new MobImage(0, 0, scrWidth-PADDING*2, height, listBox, false, false, RES_LOADING);

	retrieveBack(tempImage, card, height-PADDING*2, imageCache);
}

void GamePlayScreen::drawCardSelectStatScreen() {
	imageCache->clearImageCache();
	listBox->setEnabled(false);
	phase = P_SELECT_STAT;

	storeHeight = mainLayout->getChildren()[0]->getChildren()[0]->getHeight();
	mainLayout->getChildren()[0]->getChildren()[0]->setHeight(0);
	listBox->setHeight(scrHeight-(mainLayout->getChildren()[1]->getHeight()));
	notice->setCaption("");
	listBox->setPosition(0, 0);

	flip = true;

	notice->setCaption("");

	clearListBox();

	updateSoftKeyLayout(options, play_stat, "", mainLayout);

	int height = listBox->getHeight();

	tempImage = new MobImage(0, 0, scrWidth-PADDING*2, height, listBox, false, false, RES_LOADING);
	retrieveBack(tempImage, card, height-PADDING*2, imageCache);
}

void GamePlayScreen::drawWaitingScreen() {
	clearListBox();

	updateSoftKeyLayout(options, refresh, "", mainLayout);

	notice->setCaption("");

	Label *lbl = new Label(0, 0, scrWidth-(PADDING*2), 0, NULL);
	lbl->setFont(gFontBlack);
	lbl->setAutoSizeY(true);
	lbl->setMultiLine(true);
	lbl->setCaption(message);
	listBox->add(lbl);

	message = "";
}

#if defined(MA_PROF_SUPPORT_STYLUS)
void GamePlayScreen::pointerPressEvent(MAPoint2d point) {
	pointPressed = point;
    locateItem(point);
}

void GamePlayScreen::pointerMoveEvent(MAPoint2d point) {
    locateItem(point);
    moved++;
}

void GamePlayScreen::pointerReleaseEvent(MAPoint2d point) {
	pointReleased = point;
	if (moved <= 8 || phase==P_SELECT_STAT || phase == P_CARD_DETAILS) {
		if (right) {
			keyPressEvent(MAK_SOFTRIGHT);
		} else if (left) {
			keyPressEvent(MAK_SOFTLEFT);
		} else if (list) {
			if(phase==P_SELECT_STAT || phase == P_CARD_DETAILS){
				if(absoluteValue(pointPressed.x-pointReleased.x) >tempImage->getWidth()/100*15||absoluteValue(pointPressed.x-pointReleased.x) > 45){
					flipOrSelect = 1;
				}else{
					flipOrSelect = 0;
					currentSelectedStat = -1;
					if(phase==P_SELECT_STAT){
						for(int i = 0;i<cardStats.size();i++){
							if(flip==cardStats[i]->getFrontOrBack()){
								if(tempImage->statContains(cardStats[i]->getLeft(),cardStats[i]->getTop(),cardStats[i]->getWidth(),cardStats[i]->getHeight(),point.x, point.y)){
									currentSelectedStat = i;
								}
							}
						}
					}
				}
			}
			keyPressEvent(MAK_FIRE);
		}
	}
	moved=0;
}

void GamePlayScreen::locateItem(MAPoint2d point) {
	list = false;
	left = false;
	right = false;

	Point p;
	p.set(point.x, point.y);

	//this is for if you want a click in the list to trigger an event
	if (phase == P_SELECT_STAT || phase == P_CARD_DETAILS) {
		for(int i = 0; i < (this->getMain()->getChildren()[0]->getChildren()[2]->getChildren()).size(); i++)
		{
			if(this->getMain()->getChildren()[0]->getChildren()[2]->getChildren()[i]->contains(p))
			{
				list = true;
			}
		}
	}

	//this is for the buttons at the bottom
	for(int i = 0; i < (this->getMain()->getChildren()[1]->getChildren()).size(); i++)
	{
		if(this->getMain()->getChildren()[1]->getChildren()[i]->contains(p))
		{
			if (i == 0) {
				left = true;
				moved=0;
			}
			else if (i == 1) {
				list = true;
				moved = 0;
			}
			else if (i == 2) {
				right = true;
				moved=0;
			}
			return;
		}
	}
}
#endif
GamePlayScreen::~GamePlayScreen() {
	delete mainLayout;

	if (next != NULL) {
		delete next;
		next = NULL;
	}
	delete imageCache;

	delete [] feedLayouts;

	clearCardStats();
	parentTag="";
	cardText="";
	id="";
	description="";
	thumburl="";
	fronturl="";
	backurl="";
	error_msg="";
	statType = "";
	statDescription = "";
	cardStatId = "";
	cardName = "";
	outcome = "";
	message = "";
}

void GamePlayScreen::selectionChanged(Widget *widget, bool selected) {
	if (phase == P_SELECT_STAT) {
		if(selected) {
			((Label *)widget)->setFont(gFontBlue);
		} else {
			((Label *)widget)->setFont(gFontBlack);
		}
	}
}

void GamePlayScreen::show() {
	if (phase == P_SELECT_STAT) {
		listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(true);
	}
	Screen::show();
}

void GamePlayScreen::hide() {
	if (phase == P_SELECT_STAT) {
		listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(false);
	}
	Screen::hide();
}

void GamePlayScreen::keyPressEvent(int keyCode) {
	int selected = listBox->getSelectedIndex();
	switch(keyCode) {
		case MAK_LEFT:
		case MAK_RIGHT:
			switch(phase){
				case P_SELECT_STAT:
				case P_CARD_DETAILS:
					flip = !flip;
					int height = listBox->getHeight();
					if (tempImage->getResource() != RES_LOADING && tempImage->getResource() != RES_TEMP) {
						maDestroyObject(tempImage->getResource());
					}
					tempImage->setResource(RES_LOADING);
					tempImage->update();
					tempImage->requestRepaint();
					maUpdateScreen();
					if (flip) {
						retrieveBack(tempImage, card, height-PADDING*2, imageCache);
					}
					else {
						retrieveFront(tempImage, card, height-PADDING*2, imageCache);
					}
					currentSelectedStat=-1;
					break;
			}
			break;
		case MAK_UP:
			switch (phase) {
				case P_SELECT_STAT:
				case P_CARD_DETAILS:
					if (tempImage->getResource() != RES_LOADING && tempImage->getResource() != RES_TEMP) {
						if(cardStats.size()>0){
							if(flip==cardStats[0]->getFrontOrBack()){
								currentSelectedStat--;
								if(currentSelectedStat < 0){
									currentSelectedStat = 0;
								}
								tempImage->refreshWidget();
								tempImage->selectStat(cardStats[currentSelectedStat]->getLeft(),cardStats[currentSelectedStat]->getTop(),cardStats[currentSelectedStat]->getWidth(),cardStats[currentSelectedStat]->getHeight(), cardStats[currentSelectedStat]->getColorRed(), cardStats[currentSelectedStat]->getColorGreen(), cardStats[currentSelectedStat]->getColorBlue());
							}
						}
					}
					break;
			}
			break;
		case MAK_DOWN:
			switch (phase) {
				case P_SELECT_STAT:
				case P_CARD_DETAILS:
					if (tempImage->getResource() != RES_LOADING && tempImage->getResource() != RES_TEMP) {
						if(cardStats.size()>0){
							if(flip==cardStats[0]->getFrontOrBack()){
								if(currentSelectedStat < cardStats.size()-1){
									currentSelectedStat++;
								}
								tempImage->refreshWidget();
								tempImage->selectStat(cardStats[currentSelectedStat]->getLeft(),cardStats[currentSelectedStat]->getTop(),cardStats[currentSelectedStat]->getWidth(),cardStats[currentSelectedStat]->getHeight(), cardStats[currentSelectedStat]->getColorRed(), cardStats[currentSelectedStat]->getColorGreen(), cardStats[currentSelectedStat]->getColorBlue());
							}
						}
					}
					break;
			}
			break;
		case MAK_BACK:
		case MAK_SOFTLEFT:
			switch (phase) {
				case P_LOADING:
					previous->show();
					break;
				/*case P_CARD_DETAILS:
					resetHeights();
					drawCardListScreen(cardIndex, yOffset);
					break;*/
				default:
					if (next != NULL) {
						delete next;
						next = NULL;
					}
					feed->setGameId(gameId.c_str());
					next = new OptionsScreen(feed, OptionsScreen::ST_GAME_OPTIONS, this);
					next->show();
					break;
			}
			break;
		case MAK_FIRE:
			switch (phase) {
				case P_SELECT_STAT:
				case P_CARD_DETAILS:
					if(flipOrSelect){
						flip = !flip;
						int height = listBox->getHeight();
						if (tempImage->getResource() != RES_LOADING && tempImage->getResource() != RES_TEMP) {
							maDestroyObject(tempImage->getResource());
						}
						tempImage->setResource(RES_LOADING);
						tempImage->update();
						tempImage->requestRepaint();
						maUpdateScreen();
						if (flip) {
							retrieveBack(tempImage, card, height-PADDING*2, imageCache);
						}
						else {
							retrieveFront(tempImage, card, height-PADDING*2, imageCache);
						}
						flipOrSelect=0;
						currentSelectedStat=-1;
					}else{
						if (tempImage->getResource() != RES_LOADING && tempImage->getResource() != RES_TEMP) {
							if(currentSelectedStat>-1){
								if(flip==cardStats[currentSelectedStat]->getFrontOrBack()){
									tempImage->refreshWidget();
									tempImage->selectStat(cardStats[currentSelectedStat]->getLeft(),cardStats[currentSelectedStat]->getTop(),cardStats[currentSelectedStat]->getWidth(),cardStats[currentSelectedStat]->getHeight(), cardStats[currentSelectedStat]->getColorRed(), cardStats[currentSelectedStat]->getColorGreen(), cardStats[currentSelectedStat]->getColorBlue());

									selectStat(currentSelectedStat);
								}
							}
						}
					}
					break;
			}
			break;
		case MAK_SOFTRIGHT:
			if (!busy) {
				busy = true;
				int urlLength, res;
				char *url = NULL;
				switch (phase) {
					/*case P_CARD_DETAILS:
						//resetHeights();
						//selectCard(card);
						break;*/
					case P_SELECT_STAT:
						if(currentSelectedStat>-1){
							this->selectStat(currentSelectedStat);
						}
						break;
					case P_WAITING:
						notice->setCaption("Refreshing...");

						//work out how long the url will be, the 17 is for the & and = symbals, as well as hard coded vars
						urlLength = LOADGAME.length() + 17 + strlen(game_id) + gameId.length() + intlen(scrHeight) + intlen(scrWidth);
						url = new char[urlLength];
						memset(url,'\0',urlLength);
						sprintf(url, "%s&%s=%s&height=%d&width=%d", LOADGAME.c_str(),
								game_id, gameId.c_str(), getMaxImageHeight(), scrWidth);

						clearListBox();
						if(mHttp.isOpen()){
							mHttp.close();
						}
						mHttp = HttpConnection(this);
						res = mHttp.create(url, HTTP_GET);
						if(res < 0) {
							hasConnection = false;
							notice->setCaption("Connection error.");
						} else {
							hasConnection = true;
							mHttp.setRequestHeader(auth_user, feed->getUsername().c_str());
							mHttp.setRequestHeader(auth_pw, feed->getEncrypt().c_str());
							mHttp.finish();
						}
						break;
					case P_RESULTS:
						notice->setCaption("loading next card...");

						//work out how long the url will be, the 17 is for the & and = symbals, as well as hard coded vars
						urlLength = CONTINUEGAME.length() + 17 + strlen(game_id) + gameId.length() + intlen(scrHeight) + intlen(scrWidth);
						url = new char[urlLength];
						memset(url,'\0',urlLength);
						sprintf(url, "%s&%s=%s&height=%d&width=%d", CONTINUEGAME.c_str(),
								game_id, gameId.c_str(), getMaxImageHeight(), scrWidth);

						clearListBox();
						if(mHttp.isOpen()){
							mHttp.close();
						}
						mHttp = HttpConnection(this);
						res = mHttp.create(url, HTTP_GET);
						if(res < 0) {
							hasConnection = false;
							notice->setCaption("Connection error.");
						} else {
							hasConnection = true;
							mHttp.setRequestHeader(auth_user, feed->getUsername().c_str());
							mHttp.setRequestHeader(auth_pw, feed->getEncrypt().c_str());
							mHttp.finish();
						}
						break;
					case P_FINISHED:
						origMenu->show();
						break;
				}
				urlLength = 0;
				res = 0;
				if (url != NULL) {
					delete url;
				}
			}
			break;

	}
}

void GamePlayScreen::resetHeights() {
	mainLayout->getChildren()[0]->getChildren()[0]->setHeight(storeHeight);
	listBox->setHeight(scrHeight-(mainLayout->getChildren()[1]->getHeight()+storeHeight + 20));
}

void GamePlayScreen::selectStat(int selected) {
	char *url = NULL;
	int urlLength = 0;
	int res = 0;
	currentSelectedStat = -1;
	listBox->setEnabled(true);
	phase = P_RESULTS;

	notice->setCaption("Checking outcome...");

	//work out how long the url will be, the 4 is for the & and = symbals
	urlLength = SELECTSTAT.length() + 4 + strlen(game_id) + gameId.length() +
			strlen(stat_id) + cardStats[selected]->getCardStatId().length();
	url = new char[urlLength];
	memset(url,'\0',urlLength);
	sprintf(url, "%s&%s=%s&%s=%s", SELECTSTAT.c_str(), game_id, gameId.c_str(), stat_id, cardStats[selected]->getCardStatId().c_str());

	clearCardStats();
	clearListBox();

	if(mHttp.isOpen()){
		mHttp.close();
	}
	mHttp = HttpConnection(this);
	res = mHttp.create(url, HTTP_GET);
	if(res < 0) {
		hasConnection = false;
		notice->setCaption("Connection error.");
	} else {
		hasConnection = true;
		mHttp.setRequestHeader(auth_user, feed->getUsername().c_str());
		mHttp.setRequestHeader(auth_pw, feed->getEncrypt().c_str());
		mHttp.finish();
	}

	delete url;
}

void GamePlayScreen::httpFinished(MAUtil::HttpConnection* http, int result) {
	error_msg = "";
	if (result == 200) {
		xmlConn = XmlConnection::XmlConnection();
		xmlConn.parse(http, this, this);
	} else {
		mHttp.close();

		notice->setCaption(no_connect);
	}
}

void GamePlayScreen::connReadFinished(Connection* conn, int result) {
}

void GamePlayScreen::connRecvFinished(Connection* conn, int result) {
}

void GamePlayScreen::xcConnError(int code) {
	if (newGame) {
		newGame = false;

		notice->setCaption(loading_game);

		//work out how long the url will be, the 17 is for the & and = symbals, as well as hard coded vars
		int urlLength = LOADGAME.length() + 17 + strlen(game_id) + gameId.length() + intlen(listBox->getHeight()) + intlen(scrWidth);
		char *url = new char[urlLength];
		memset(url,'\0',urlLength);
		sprintf(url, "%s&%s=%s&height=%d&width=%d", LOADGAME.c_str(),
				game_id, gameId.c_str(), getMaxImageHeight(), scrWidth);

		if(mHttp.isOpen()){
			mHttp.close();
		}
		mHttp = HttpConnection(this);
		int res = mHttp.create(url, HTTP_GET);
		if(res < 0) {
			hasConnection = false;
			notice->setCaption("Connection error.");
		} else {
			hasConnection = true;
			mHttp.setRequestHeader(auth_user, feed->getUsername().c_str());
			mHttp.setRequestHeader(auth_pw, feed->getEncrypt().c_str());
			mHttp.finish();
		}
		delete [] url;
	}
}

void GamePlayScreen::mtxEncoding(const char* ) {
}

void GamePlayScreen::mtxTagStart(const char* name, int len) {
	lprintfln("mtxTagStart: %s", name);
	parentTag = name;
}

void GamePlayScreen::mtxTagAttr(const char* attrName, const char* attrValue) {
	lprintfln("mtxTagAttr: mtxTagAttr:%s, attrValue: %s", attrName, attrValue);
	if(!strcmp(parentTag.c_str(), xml_cardstat)) {
		if(!strcmp(attrName, xml_cardstat_id)) {
			cardStatId += attrValue;
		}else if(!strcmp(attrName, xml_desc)) {
			statDesc += attrValue;
		}else if(!strcmp(attrName, xml_ival)) {
			statIVal += attrValue;
		}else if(!strcmp(attrName, xml_top)) {
			statTop = atoi(attrValue);
		}else if(!strcmp(attrName, xml_left)) {
			statLeft = atoi(attrValue);
		}else if(!strcmp(attrName, xml_width)) {
			statWidth = atoi(attrValue);
		}else if(!strcmp(attrName, xml_height)) {
			statHeight = atoi(attrValue);
		}else if(!strcmp(attrName, xml_frontorback)) {
			statFrontOrBack = atoi(attrValue);
		}else if(!strcmp(attrName, xml_red)) {
			statRed = atoi(attrValue);
		}else if(!strcmp(attrName, xml_green)) {
			statGreen = atoi(attrValue);
		}else if(!strcmp(attrName, xml_blue)) {
			statBlue = atoi(attrValue);
		}
	}
}

void GamePlayScreen::mtxTagData(const char* data, int len) {
	lprintfln("mtxTagData: %s", data);
	if(!strcmp(parentTag.c_str(), xml_cardid)) {
		id += data;
	} else if(!strcmp(parentTag.c_str(), xml_carddescription)) {
		description += data;
	} else if(!strcmp(parentTag.c_str(), xml_gameplayercard_id)) {
		gamePlayerCardId += data;
	} else if(!strcmp(parentTag.c_str(), xml_thumburl)) {
		thumburl += data;
	} else if(!strcmp(parentTag.c_str(), xml_fronturl)) {
		fronturl += data;
	} else if(!strcmp(parentTag.c_str(), xml_backurl)) {
		backurl += data;
	} else if(!strcmp(parentTag.c_str(), xml_error)) {
		error_msg += data;
	} else if(!strcmp(parentTag.c_str(), xml_game_id)) {
		gameId += data;
	} else if(!strcmp(parentTag.c_str(), xml_stat_type)) {
		statType += data;
	} else if(!strcmp(parentTag.c_str(), xml_stat_description)) {
		statDescription += data;
	} else if(!strcmp(parentTag.c_str(), xml_cardstat_id)) {
		cardStatId += data;
	} else if(!strcmp(parentTag.c_str(), xml_card_name)) {
		cardName += data;
	} else if(!strcmp(parentTag.c_str(), xml_player_score)) {
		userScore += data;
	} else if(!strcmp(parentTag.c_str(), xml_opponent_score)) {
		opponentScore += data;
	} else if(!strcmp(parentTag.c_str(), xml_explanation)) {
		explanation += data;
	} else if(!strcmp(parentTag.c_str(), xml_outcome)) {
		outcome += data;
	} else if(!strcmp(parentTag.c_str(), xml_message)) {
		message += data;
	} else if (!strcmp(parentTag.c_str(), xml_phase)) {
		listBox->setEnabled(true);
		if (!strcmp(data, phase_waiting)) {
			phase = P_WAITING;
		}
		else if (!strcmp(data, phase_stat)) {
			phase = P_SELECT_STAT;
		}
		else if (!strcmp(data, phase_result)) {
			phase = P_RESULTS;
		}
		else if (!strcmp(data, phase_finished)) {
			phase = P_FINISHED;
		}
	}
}

void GamePlayScreen::mtxTagEnd(const char* name, int len) {
	lprintfln("mtxTagEnd: %s", name);
	if (!strcmp(name, xml_backurl)) {
		Card *newCard = new Card();
		newCard->setText(description.c_str());
		newCard->setThumb(thumburl.c_str());
		newCard->setFront(fronturl.c_str());
		newCard->setBack(backurl.c_str());
		newCard->setId(id.c_str());
		newCard->setGamePlayerCardId(gamePlayerCardId.c_str());
		newCard->setStats(cardStats);
		if(loadingGame){
			loadingGame = false;
			card = newCard;
		}
		id = "";
		description = "";
		thumburl = "";
		fronturl = "";
		backurl = "";
		gamePlayerCardId = "";
	} else if (!strcmp(name, xml_cardstat)) {
		newStat = new Stat();
		newStat->setCardStatId(cardStatId.c_str());
		newStat->setDesc(statDesc.c_str());
		newStat->setDisplay(statDescription.c_str());
		newStat->setIVal(statIVal.c_str());
		newStat->setTop(statTop);
		newStat->setLeft(statLeft);
		newStat->setWidth(statWidth);
		newStat->setHeight(statHeight);
		newStat->setFrontOrBack(statFrontOrBack);
		newStat->setColorRed(statRed);
		newStat->setColorGreen(statGreen);
		newStat->setColorBlue(statBlue);
		cardStats.add(newStat);
		cardStatId = "";
		statDescription = "";
		statType = "";
		statDesc = "";
		statIVal = "";
	} else if (!strcmp(name, xml_error)) {
		notice->setCaption(error_msg.c_str());
	} else if (!strcmp(name, xml_game) || !strcmp(name, xml_cardstats) || !strcmp(name, xml_results)) {
		if (!newGame) {
			busy = false;
			switch (phase) {
				case P_WAITING:
					drawWaitingScreen();
					break;
				case P_SELECT_STAT:
					drawCardSelectStatScreen();
					//drawStatListScreen();
					break;
				case P_RESULTS:
					drawResultsScreen();
					break;
				case P_FINISHED:
					drawResultsScreen();
					break;
			}
		}
	} else {
		notice->setCaption("");
	}
}

void GamePlayScreen::clearCardStats() {
	for (int i = 0; i < cardStats.size(); i++) {
		if (cardStats[i] != NULL) {
			//delete cardStats[i];
			cardStats[i] = NULL;
		}
	}
	cardStats.clear();
}

void GamePlayScreen::mtxParseError() {
}

void GamePlayScreen::mtxEmptyTagEnd() {
}

void GamePlayScreen::mtxTagStartEnd() {
}
