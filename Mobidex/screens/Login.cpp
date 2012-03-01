#include <mastdlib.h>

#include "Login.h"
#include "../utils/Util.h"
#include "AlbumLoadScreen.h"

static country countries[] =
{
	{ "7840", "Abkhazia" },
	{ "7940", "Abkhazia" },
	{ "99544", "Abkhazia" },
	{ "93", "Afghanistan" },
	{ "358", "Aland Islands" },
	{ "355", "Albania" },
	{ "213", "Algeria" },
	{ "1684", "American Samoa" },
	{ "376", "Andorra" },
	{ "244", "Angola" },
	{ "1264", "Anguilla" },
	{ "1268", "Antigua and Barbuda" },
	{ "54", "Argentina" },
	{ "374", "Armenia" },
	{ "297", "Aruba" },
	{ "247", "Ascension" },
	{ "61", "Australia" },
	{ "672", "Australian External Territories" },
	{ "43", "Austria" },
	{ "994", "Azerbaijan" },
	{ "1242", "Bahamas" },
	{ "973", "Bahrain" },
	{ "880", "Bangladesh" },
	{ "1246", "Barbados" },
	{ "1268", "Barbuda" },
	{ "375", "Belarus" },
	{ "32", "Belgium" },
	{ "501", "Belize" },
	{ "229", "Benin" },
	{ "1441", "Bermuda" },
	{ "975", "Bhutan" },
	{ "591", "Bolivia" },
	{ "5997", "Bonaire" },
	{ "387", "Bosnia and Herzegovina" },
	{ "267", "Botswana" },
	{ "55", "Brazil" },
	{ "246", "British Indian Ocean Territory" },
	{ "1284", "British Virgin Islands" },
	{ "673", "Brunei Darussalam" },
	{ "359", "Bulgaria" },
	{ "226", "Burkina Faso" },
	{ "257", "Burundi" },
	{ "855", "Cambodia" },
	{ "257", "Cameroon" },
	{ "1", "Canada" },
	{ "238", "Cape Verde" },
	{ "5993", "Caribbean Netherlands" },
	{ "5994", "Caribbean Netherlands" },
	{ "5997", "Caribbean Netherlands" },
	{ "1345", "Cayman Islands" },
	{ "236", "Central African Republic" },
	{ "235", "Chad" },
	{ "64", "Chatham Island (New Zealand)" },
	{ "56", "Chile" },
	{ "86", "China" },
	{ "61", "Christmas Island" },
	{ "61", "Cocos (Keeling) Islands" },
	{ "57", "Colombia" },
	{ "269", "Comoros" },
	{ "242", "Congo (Brazzaville)" },
	{ "243", "Congo, The Democratic Republic of the (Zaire)" },
	{ "682", "Cook Islands" },
	{ "506", "Costa Rica" },
	{ "225", "C�te d'Ivoire" },
	{ "385", "Croatia" },
	{ "53", "Cuba" },
	{ "5399", "Cuba (Guantanamo Bay)" },
	{ "5999", "Cura�ao" },
	{ "357", "Cyprus" },
	{ "420", "Czech Republic" },
	{ "45", "Denmark" },
	{ "246", "Diego Garcia" },
	{ "253", "Djibouti" },
	{ "1767", "Dominica" },
	{ "1809", "Dominican Republic" },
	{ "1829", "Dominican Republic" },
	{ "1849", "Dominican Republic" },
	{ "670", "East Timor" },
	{ "56", "Easter Island" },
	{ "593", "Ecuador" },
	{ "20", "Egypt" },
	{ "503", "El Salvador" },
	{ "8812", "Ellipso (Mobile Satellite service)" },
	{ "8813", "Ellipso (Mobile Satellite service)" },
	{ "88213", "EMSAT (Mobile Satellite service)" },
	{ "240", "Equatorial Guinea" },
	{ "291", "Eritrea" },
	{ "372", "Estonia" },
	{ "251", "Ethiopia" },
	{ "500", "Falkland Islands (Malvinas)" },
	{ "298", "Faroe Islands" },
	{ "679", "Fiji" },
	{ "358", "Finland" },
	{ "33", "France" },
	{ "596", "French Antilles" },
	{ "594", "French Guiana" },
	{ "689", "French Polynesia" },
	{ "241", "Gabon" },
	{ "220", "Gambia" },
	{ "995", "Georgia" },
	{ "49", "Germany" },
	{ "233", "Ghana" },
	{ "350", "Gibraltar" },
	{ "881", "Global Mobile Satellite System (GMSS)" },
	{ "8818", "Globalstar (Mobile Satellite Service)" },
	{ "8819", "Globalstar (Mobile Satellite Service)" },
	{ "30", "Greece" },
	{ "299", "Greenland" },
	{ "1473", "Grenada" },
	{ "590", "Guadeloupe" },
	{ "1671", "Guam" },
	{ "502", "Guatemala" },
	{ "44", "Guernsey" },
	{ "224", "Guinea" },
	{ "245", "Guinea-Bissau" },
	{ "592", "Guyana" },
	{ "509", "Haiti" },
	{ "39066", "Holy See (Vatican City State)" },
	{ "379", "Holy See (Vatican City State)" },
	{ "504", "Honduras" },
	{ "852", "Hong Kong" },
	{ "36", "Hungary" },
	{ "354", "Iceland" },
	{ "8810", "ICO Global (Mobile Satellite Service)" },
	{ "8811", "ICO Global (Mobile Satellite Service)" },
	{ "91", "India" },
	{ "62", "Indonesia" },
	{ "870", "Inmarsat SNAC" },
	{ "800", "International Freephone Service" },
	{ "808", "International Shared Cost Service (ISCS)" },
	{ "98", "Iran" },
	{ "964", "Iraq" },
	{ "353", "Ireland" },
	{ "8816", "Iridium (Mobile Satellite service)" },
	{ "8817", "Iridium (Mobile Satellite service)" },
	{ "44", "Isle of Man" },
	{ "972", "Israel" },
	{ "39", "Italy" },
	{ "1876", "Jamaica" },
	{ "81", "Japan" },
	{ "44", "Jersey" },
	{ "962", "Jordan" },
	{ "76", "Kazakhstan" },
	{ "77", "Kazakhstan" },
	{ "686", "Kiribati" },
	{ "850", "North Korea" },
	{ "82", "South Korea" },
	{ "965", "Kuwait" },
	{ "996", "Kyrgyzstan" },
	{ "856", "Laos" },
	{ "371", "Latvia" },
	{ "961", "Lebanon" },
	{ "266", "Lesotho" },
	{ "231", "Liberia" },
	{ "218", "Libya" },
	{ "423", "Liechtenstein" },
	{ "370", "Lithuania" },
	{ "352", "Luxembourg" },
	{ "853", "Macau" },
	{ "389", "Macedonia" },
	{ "261", "Madagascar" },
	{ "256", "Malawi" },
	{ "60", "Malaysia" },
	{ "960", "Maldives" },
	{ "223", "Mali" },
	{ "356", "Malta" },
	{ "692", "Marshall Islands" },
	{ "596", "Martinique" },
	{ "222", "Mauritania" },
	{ "230", "Mauritius" },
	{ "262", "Mayotte" },
	{ "52", "Mexico" },
	{ "691", "Micronesia, Federated States of" },
	{ "1808", "Midway Island" },
	{ "373", "Moldova" },
	{ "377", "Monaco" },
	{ "976", "Mongolia" },
	{ "382", "Montenegro" },
	{ "1664", "Montserrat" },
	{ "212", "Morocco" },
	{ "258", "Mozambique" },
	{ "95", "Myanmar" },
	{ "264", "Namibia" },
	{ "674", "Nauru" },
	{ "977", "Nepal" },
	{ "31", "Netherlands" },
	{ "1869", "Nevis" },
	{ "687", "New Caledonia" },
	{ "64", "New Zealand" },
	{ "505", "Nicaragua" },
	{ "227", "Niger" },
	{ "234", "Nigeria" },
	{ "683", "Niue" },
	{ "672", "Norfolk Island" },
	{ "1670", "Northern Mariana Islands" },
	{ "47", "Norway" },
	{ "968", "Oman" },
	{ "92", "Pakistan" },
	{ "680", "Palau" },
	{ "970", "Palestinian territories" },
	{ "507", "Panama" },
	{ "675", "Papua New Guinea" },
	{ "595", "Paraguay" },
	{ "51", "Peru" },
	{ "63", "Philippines" },
	{ "48", "Poland" },
	{ "351", "Portugal" },
	{ "1787", "Puerto Rico" },
	{ "1939", "Puerto Rico" },
	{ "974", "Qatar" },
	{ "262", "R�union" },
	{ "40", "Romania" },
	{ "7", "Russia" },
	{ "250", "Rwanda" },
	{ "5994", "Saba" },
	{ "590", "Saint Barth�lemy" },
	{ "290", "Saint Helena and Tristan da Cunha" },
	{ "1869", "Saint Kitts and Nevis" },
	{ "1758", "Saint Lucia" },
	{ "590", "Saint Martin (French)" },
	{ "508", "Saint Pierre and Miquelon" },
	{ "1784", "Saint Vincent and the Grenadines" },
	{ "685", "Samoa" },
	{ "378", "San Marino" },
	{ "239", "S�o Tom� and Pr�ncipe" },
	{ "966", "Saudi Arabia" },
	{ "221", "Senegal" },
	{ "381", "Serbia" },
	{ "248", "Seychelles" },
	{ "232", "Sierra Leone" },
	{ "65", "Singapore" },
	{ "5993", "Sint Eustatius" },
	{ "1721", "Sint Maarten (Dutch)" },
	{ "421", "Slovakia" },
	{ "386", "Slovenia" },
	{ "677", "Solomon Islands" },
	{ "252", "Somalia" },
	{ "27", "South Africa" },
	{ "500", "South Georgia and the South Sandwich Islands" },
	{ "211", "South Sudan" },
	{ "34", "Spain" },
	{ "94", "Sri Lanka" },
	{ "249", "Sudan" },
	{ "597", "Suriname" },
	{ "47", "Svalbard and Jan Mayen" },
	{ "268", "Swaziland" },
	{ "46", "Sweden" },
	{ "41", "Switzerland" },
	{ "963", "Syria" },
	{ "886", "Taiwan" },
	{ "992", "Tajikistan" },
	{ "255", "Tanzania" },
	{ "66", "Thailand" },
	{ "88216", "Thuraya (Mobile Satellite service)" },
	{ "228", "Togo" },
	{ "690", "Tokelau" },
	{ "676", "Tonga" },
	{ "1868", "Trinidad and Tobago" },
	{ "216", "Tunisia" },
	{ "90", "Turkey" },
	{ "993", "Turkmenistan" },
	{ "1649", "Turks and Caicos Islands" },
	{ "688", "Tuvalu" },
	{ "256", "Uganda" },
	{ "380", "Ukraine" },
	{ "971", "United Arab Emirates" },
	{ "44", "United Kingdom" },
	{ "1", "United States" },
	{ "878", "Universal Personal Telecommunications " },
	{ "598", "Uruguay" },
	{ "998", "Uzbekistan" },
	{ "678", "Vanuatu" },
	{ "58", "Venezuela" },
	{ "39066", "Vatican City State" },
	{ "379", "Vatican City State" },
	{ "84", "Vietnam" },
	{ "1340", "Virgin Islands, US" },
	{ "1808", "Wake Island" },
	{ "681", "Wallis and Futuna" },
	{ "967", "Yemen" },
	{ "260", "Zambia" },
	{ "255", "Zanzibar" },
	{ "263", "Zimbabwe" }
};

Login::Login(Feed *feed, Screen *previous, int screen) : mHttp(this), feed(feed), prev(previous), screen(screen) {
	lprintfln("Login::Memory Heap %d, Free Heap %d", heapTotalMemory(), heapFreeMemory());
	moved = 0;
	isBusy = false;
	termsMenu == NULL;

	response = "";

	mainLayout = Util::createMainLayout("", "", "", true);

	mainLayout->setDrawBackground(TRUE);
	listBox = (KineticListBox*) mainLayout->getChildren()[0]->getChildren()[2];
	//Util::setPadding(listBox);
	notice = (Label*) mainLayout->getChildren()[0]->getChildren()[1];
	notice->setMultiLine(true);

	switch (screen) {
		case S_LOGIN:
			drawLoginScreen();
			break;
		case S_REGISTER:
			drawRegisterScreen();
			break;
	}

	if (feed->getUnsuccessful() != "Success") {
		label->setCaption(feed->getUnsuccessful());
	}
	touch = "false";
	this->setMain(mainLayout);
}

Login::~Login() {
	clearListBox();
	listBox->clear();
	delete mainLayout;
	error_msg = "";
	parentTag="";
	conCatenation="";
	value="";
	value1="";
	value2="";
	convertAsterisk="";
	underscore="";
	username="";
	credits="";
	encrypt="";
	error_msg="";
	email="";
	handle="";
	touch="";

	if (termsMenu != NULL) {
		delete termsMenu;
		termsMenu = NULL;
	}
}

void Login::drawLoginScreen() {
	//listBox->setYOffset(0);
	moved = 0;
	screen = S_LOGIN;
	clearListBox();

	if ((strcmp(feed->getRegistered().c_str(), "1") == 0)) {
		Util::updateSoftKeyLayout("Log In", "Exit", "", mainLayout);
	} else {
		Util::updateSoftKeyLayout("Log In", "Back", "", mainLayout);
	}
	notice->setCaption("");

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "Username:", 0, Util::getDefaultFont());
	listBox->add(label);

	label = Util::createEditLabel("");
	editBoxLogin = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "", L"Username:");
	editBoxLogin->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "Password:", 0, Util::getDefaultFont());
	listBox->add(label);

	label = Util::createEditLabel("");
	editBoxPass = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "", L"Password:");
	editBoxPass->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "", 0, Util::getDefaultFont());
	listBox->add(label);

	termsLink = new Label(0,0, scrWidth-(PADDING*2), DEFAULT_LABEL_HEIGHT, NULL, "Forgot password", 0, Util::getFontBlue());
	termsLink->setSkin(Util::getSkinDropDownItem());
	Util::setPadding(termsLink);
	termsLink->addWidgetListener(this);
	listBox->add(termsLink);

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "", 0, Util::getDefaultFont());
	listBox->add(label);
	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "", 0, Util::getDefaultFont());
	listBox->add(label);

	listBox->setSelectedIndex(1);
}

void Login::drawRegisterScreen() {
	listBox->setYOffset(0);
	moved = 0;
	screen = S_REGISTER;
	clearListBox();

	Util::updateSoftKeyLayout("Register", "Back", "", mainLayout);
	notice->setCaption("");

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "Username:", 0, Util::getDefaultFont());
	listBox->add(label);

	label = Util::createEditLabel("");
	editBoxLogin = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "", L"Username:");
	editBoxLogin->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "Password:", 0, Util::getDefaultFont());
	listBox->add(label);

	label = Util::createEditLabel("");
	editBoxPass = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "", L"Password:");
	editBoxPass->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "Full Name:", 0, Util::getDefaultFont());
	listBox->add(label);

	label = Util::createEditLabel("");
	editBoxFullname = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "", L"Full Name:");
	editBoxFullname->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "International Cell Number", 0, Util::getDefaultFont());
	listBox->add(label);

	label = Util::createEditLabel("");
	editBoxCell = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_NUMERIC, label, "", L"Cell Number:");
#if defined(MA_PROF_SUPPORT_STYLUS)

#else
	editBoxCell->setInputMode(NativeEditBox::IM_NUMBERS);
#endif
	editBoxCell->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "Email:", 0, Util::getDefaultFont());
	listBox->add(label);

	label = Util::createEditLabel("");
	editBoxEmail = new NativeEditBox(0, 0, label->getWidth()-PADDING*2, label->getHeight()-PADDING*2, 64, MA_TB_TYPE_ANY, label, "", L"Email:");
	editBoxEmail->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "", 0, Util::getDefaultFont());
	listBox->add(label);

	termsLayout = new Layout(0, 0, scrWidth-(PADDING*2), DEFAULT_LABEL_HEIGHT, listBox, 2, 1);
	termsLayout->setHorizontalAlignment(Layout::HA_CENTER);
	termsLayout->setVerticalAlignment(Layout::VA_CENTER);
	termsLayout->setSkin(Util::getSkinDropDownItem());
	termsLayout->setDrawBackground(true);
	termsLayout->setPaddingLeft(PADDING);
	termsLink = new Label(0, 0, 0, 0, termsLayout, "Terms and conditions ", 0, Util::getFontBlue());
	termsLink->setAutoSizeX(true);
	termsLink->setAutoSizeY(true);
	termsLink->setDrawBackground(false);
	termsBox = new CheckBox(0, 0, 35, 35, termsLayout);
	termsBox->setDrawBackground(false);

	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "", 0, Util::getDefaultFont());
	listBox->add(label);
	label = new Label(0,0, scrWidth-PADDING*2, 24, NULL, "", 0, Util::getDefaultFont());
	listBox->add(label);

	listBox->setSelectedIndex(1);

	termsMenu = new MenuScreen(RES_BLANK);
	termsMenu->setMenuWidth(140);
	termsMenu->setMarginX(5);
	termsMenu->setMarginY(5);
	termsMenu->setDock(MenuScreen::MD_CENTER);
	termsMenu->setMenuFontSel(Util::getFontBlack());
	termsMenu->setMenuFontUnsel(Util::getFontWhite());
	termsMenu->setMenuSkin(Util::getSkinDropDownItem());
	termsMenu->addItem("View T's and C's");
	termsMenu->addItem("Accept");
	termsMenu->addItem("Reject");
	termsMenu->setListener(this);

}

void Login::clearListBox() {
	for (int i = 0; i < listBox->getChildren().size(); i++) {
		tempWidgets.add(listBox->getChildren()[i]);
	}
	listBox->clear();

	for (int j = 0; j < tempWidgets.size(); j++) {
		tempWidgets[j]->setSelected(false);
		delete tempWidgets[j];
		tempWidgets[j] = NULL;
	}
	tempWidgets.clear();
}

void Login::selectionChanged(Widget *widget, bool selected) {
	if (widget->getChildren().size() > 0) {
		if(selected) {
			widget->getChildren()[0]->setSelected(true);
		} else {
			widget->getChildren()[0]->setSelected(false);
		}
	}
}

void Login::pointerPressEvent(MAPoint2d point)
{
    locateItem(point);
}

void Login::pointerMoveEvent(MAPoint2d point)
{
    locateItem(point);
    moved++;
}

void Login::pointerReleaseEvent(MAPoint2d point)
{
	if (moved <= 8) {
		if (screen == S_REGISTER && editBoxEmail->isSelected()) {
			editBoxEmail->activate();
		}

		if (right) {
			keyPressEvent(MAK_SOFTRIGHT);
		} else if (left) {
			keyPressEvent(MAK_SOFTLEFT);
		} else if (list) {
			keyPressEvent(MAK_FIRE);
		}
	}
	else {
		moved = 0;
	}
}

void Login::locateItem(MAPoint2d point)
{
	touch = "true";
	/*if (feed->setTouch(touch.c_str())) {
		saveData(FEED, feed->getAll().c_str());
	}*/
	list = false;
	left = false;
	right = false;
	mid = false;

    Point listP, p;
    p.set(point.x, point.y);
    listP.set(point.x, point.y - (listBox->getYOffset()>>16));
    for(int i = 0; i < (listBox->getChildren()).size() &&
    	!mainLayout->getChildren()[1]->contains(p); i++)
    {
        if(listBox->getChildren()[i]->contains(listP))
        {
        	if (moved < 1) listBox->setSelectedIndex(i);
        	list = true;
            return;
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
void Login::show() {
	listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(true);
	Screen::show();
}

void Login::hide() {
    listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(false);
	Screen::hide();
}

void Login::keyPressEvent(int keyCode) {
	error = false;
	int index = listBox->getSelectedIndex();
	switch(keyCode) {
		case MAK_FIRE:
			switch (screen) {
				case S_LOGIN:
					if (termsLink->isSelected()) {
						maPlatformRequest("http://www.mobidex.biz/forgotpassword");
					}
					break;
				case S_REGISTER:
					if (termsLayout->isSelected()) {
						termsMenu->show();
					}
					break;
			}
			break;
		case MAK_SOFTLEFT:
			if (!isBusy) {
				switch (screen) {
					case S_LOGIN:
						if (editBoxLogin->getText()!="" & editBoxPass->getText()!="") {
							isBusy = true;
							response = "";
							notice->setCaption("Please wait, logging in...");
							conCatenation = editBoxPass->getText().c_str();
							value = Util::base64_encode(reinterpret_cast<const unsigned char*>(conCatenation.c_str()),conCatenation.length());
							feed->setEncrypt(value.c_str());
							feed->setUsername(editBoxLogin->getText().c_str());
							feed->setUnsuccessful("true");
							mHttp = HttpConnection(this);
							int urlLength = 71 + URLSIZE;
							char *url = new char[urlLength+1];
							memset(url,'\0',urlLength+1);
							sprintf(url, "%s?userdetails=1", URL);
							int res = mHttp.create(url, HTTP_GET);

							if(res < 0) {
								notice->setCaption("Unable to connect, try again later...");
							} else {
								mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
								mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
								feed->addHttp();
								mHttp.finish();
							}
							conCatenation = "";
							value = "";
						} else {
							notice->setCaption("Ensure that you have entered your username and password.");
						}
						break;
					case S_REGISTER:
						notice->setCaption("");
						int countryIndex = checkCountryCode();
						if (editBoxLogin->getText().length() < 6) {
							notice->setCaption("Your username needs to be at least 6 characters long");
						}
						else if (editBoxPass->getText().length() < 6) {
							notice->setCaption("Your password needs to be at least 6 characters long");
						}
						else if (editBoxFullname->getText().length() < 1) {
							notice->setCaption("Please enter your name");
						}
						else if (editBoxCell->getText().length() < 10) {
							notice->setCaption("Your cell needs to be at least 10 numbers long");
						}
						else if (editBoxEmail->getText().length() < 1) {
							notice->setCaption("You need to enter an email address");
						}
						else if (countryIndex == -1) {
							notice->setCaption("Please enter your cell number in the international format");
						}
						else if (!termsBox->isChecked()) {
							notice->setCaption("You need to accept the terms and conditions to register");
						}
						else {
							response = "";
							isBusy = true;
							notice->setCaption("Attempting to register user...");

							String encodedName = Util::base64_encode(reinterpret_cast<const unsigned char*>(editBoxFullname->getText().c_str()),
								editBoxFullname->getText().length());

							String encodedCountry = Util::base64_encode(reinterpret_cast<const unsigned char*>(countries[countryIndex].country),
								strlen(countries[countryIndex].country));

							String encodedNumber = Util::base64_encode(reinterpret_cast<const unsigned char*>(editBoxCell->getText().c_str()),
									editBoxCell->getText().length());

							char *url = NULL;
							//work out how long the url will be, the 10 is for the & and = symbols
							int urlLength = 0;
							urlLength = 100 + urlLength + encodedName.length()
									+ editBoxLogin->getText().length() + encodedNumber.length()
									+ editBoxPass->getText().length() + editBoxEmail->getText().length() + encodedCountry.length() + 10;
							url = new char[urlLength];
							memset(url,'\0',urlLength);
							sprintf(url, "%s?registeruser=1&name=%s&username=%s&cell=%s&password=%s&email=%s&country=%s", URL, encodedName.c_str(),
									editBoxLogin->getText().c_str(), encodedNumber.c_str(),
									editBoxPass->getText().c_str(), editBoxEmail->getText().c_str(), encodedCountry.c_str());
							mHttp = HttpConnection(this);
							int res = mHttp.create(url, HTTP_GET);
							if(res < 0) {
								notice->setCaption("Unable to connect, try again later...");
							} else {
								mHttp.setRequestHeader("AUTH_USER", feed->getUsername().c_str());
								mHttp.setRequestHeader("AUTH_PW", feed->getEncrypt().c_str());
								feed->addHttp();
								mHttp.finish();
							}
							delete url;
							encodedName = "";
						}
						break;
				}
			}
			break;
		case MAK_BACK:
		case MAK_SOFTRIGHT:
			if ((strcmp(feed->getRegistered().c_str(), "1") == 0)) {
				maExit(1);
			} else {
				disableEditBoxes();
				prev->show();
			}
			break;
		case MAK_UP:
			if (index == 3) {
				listBox->setSelectedIndex(0);
				listBox->setSelectedIndex(1);
			} else if (index-2 > 0) {
				listBox->setSelectedIndex(index-2);
			} else {
				listBox->setSelectedIndex(listBox->getChildren().size()-1);
				listBox->setSelectedIndex(listBox->getChildren().size()-3);
			}
			break;
		case MAK_DOWN:
			if (index+2 < listBox->getChildren().size()) {
				listBox->setSelectedIndex(index+2);
			} else {
				listBox->setSelectedIndex(0);
				listBox->setSelectedIndex(1);
			}
			break;
	}
}

//returns -1 if the country code wasnt found, otherwise it returns the index of the country
int Login::checkCountryCode() {
	int found = -1;
	int countryIndex;
	int countryCount = sizeof(countries)/sizeof(country);
	for (countryIndex = 0; countryIndex < countryCount; countryIndex++) {
		int codeLength = strlen(countries[countryIndex].code);
		if (editBoxCell->getText().length() == codeLength + 9) {
			if (strcmp((editBoxCell->getText().substr(0, codeLength)).c_str(), countries[countryIndex].code) == 0) {
				found = 1;
				break;
			}
		}
	}

	if (found > 0) {
		return countryIndex;
	}
	else {
		return found;
	}
}

void Login::disableEditBoxes() {
	switch (screen) {
		case S_LOGIN:
			editBoxLogin->disableListener();
			editBoxPass->disableListener();
			break;
		case S_REGISTER:
			editBoxLogin->disableListener();
			editBoxPass->disableListener();
			editBoxFullname->disableListener();
			editBoxCell->disableListener();
			editBoxEmail->disableListener();
			break;
	}
}

void Login::httpFinished(MAUtil::HttpConnection* http, int result) {
	if (result == 200) {
		xmlConn = XmlConnection::XmlConnection();
		xmlConn.parse(http, this, this);
	} else {
		mHttp.close();
		notice->setCaption("Unable to connect, try again later...");
		isBusy = false;
		feed->remHttp();
	}
}

void Login::connReadFinished(Connection* conn, int result) {}

void Login::xcConnError(int code) {
	feed->remHttp();
	isBusy = false;
}

void Login::mtxEncoding(const char* ) {
}

void Login::mtxTagStart(const char* name, int len) {
	parentTag = name;
	if (!strcmp(name, "usercategories")) {
		album = new Albums();
	}
}

void Login::mtxTagAttr(const char* attrName, const char* attrValue) {
}

void Login::mtxTagData(const char* data, int len) {
	if(!strcmp(parentTag.c_str(), "username")) {
		username = data;
	} else if(!strcmp(parentTag.c_str(), "credits")) {
		credits = data;
	} else if(!strcmp(parentTag.c_str(), "email")) {
		email = data;
	} else if(!strcmp(parentTag.c_str(), "name")) {
		handle = data;
	} else if(!strcmp(parentTag.c_str(), "error")) {
		error_msg = data;
	} else if (!strcmp(parentTag.c_str(), "result")) {
		response += data;
	} else if (!strcmp(parentTag.c_str(), "usercategories")) {
		album->clearAll();
	} else if(!strcmp(parentTag.c_str(), "albumname")) {
		temp1 += data;
	} else if(!strcmp(parentTag.c_str(), "albumid")) {
		temp += data;
	} else if (!strcmp(parentTag.c_str(), "hascards")) {
		hasCards += data;
	} else if (!strcmp(parentTag.c_str(), "updated")) {
		updated += data;
	}
}

void Login::mtxTagEnd(const char* name, int len) {
	if(!strcmp(name, "status")) {
		feed->setCredits(credits.c_str());
		feed->setHandle(handle.c_str());
		feed->setEmail(email.c_str());
		feed->setUnsuccessful("Success");
		feed->setTouch(touch.c_str());
		feed->setRegistered("1");
		int seconds = maLocalTime();
		int secondsLength = Util::intlen(seconds);
		char *secString = new char[secondsLength];
		memset(secString,'\0',secondsLength);
		sprintf(secString, "%d", "seconds");
		feed->setSeconds(secString);
		delete secString;
		username,error_msg= "";
		Util::saveData("fd.sav", feed->getAll().c_str());
		String albums = "";
		Util::getData("lb.sav", albums);
		feed->setAlbum(albums.c_str());
		albums = "";
		/*if (next != NULL) {
			delete next;
		}*/
		next = new AlbumLoadScreen(feed);
		next->show();
	} else if(!strcmp(name, "error")) {
		error = true;
		feed->setUnsuccessful(error_msg.c_str());
		notice->setCaption(error_msg.c_str());
	} else if (!strcmp(name, "result")) {
		isBusy = false;
		error = true;
		notice->setCaption(response);
	} else if(!strcmp(name, "album")) {
		album->addAlbum(temp.c_str(), temp1, (hasCards=="true"), (updated=="1"));
		temp1 = "";
		temp = "";
		hasCards = "";
		updated = "";
	} else if (!strcmp(name, "usercategories")) {
		this->feed->getAlbum()->setAll(album->getAll().c_str());
		Util::saveData("lb.sav", album->getAll().c_str());

		feed->setEncrypt(Util::base64_encode(reinterpret_cast<const unsigned char*>(editBoxPass->getText().c_str()), editBoxPass->getText().length()).c_str());
		feed->setUsername(editBoxLogin->getText().c_str());
		feed->setCredits("0");
		feed->setHandle("");
		feed->setEmail(editBoxEmail->getText().c_str());
		feed->setUnsuccessful("Success");
		feed->setTouch("");
		int seconds = maLocalTime();
		int secondsLength = Util::intlen(seconds);
		char *secString = new char[secondsLength];
		memset(secString,'\0',secondsLength);
		sprintf(secString, "%d", seconds);
		feed->setSeconds(secString);
		delete secString;
		username,error_msg= "";
		Util::saveData("fd.sav", feed->getAll().c_str());

		/*if (next != NULL) {
			delete next;
		}*/
		next = new AlbumLoadScreen(feed, album);
		next->show();
	} else {
		if (!error) {
			if (notice != NULL) {
				notice->setCaption("");
			}
		}
	}
}
void Login::cleanup() {
	delete mainLayout;

	parentTag = "";
	conCatenation = "";
	value = "";
	value1 = "";
	value2 = "";
	convertAsterisk = "";
	underscore = "";
	username = "";
	credits = "";
	encrypt = "";
	error_msg = "";
	email = "";
	handle = "";
	touch = "";
}

void Login::mtxParseError(int) {
}

void Login::mtxEmptyTagEnd() {
}

void Login::mtxTagStartEnd() {
}

void Login::menuOptionSelected(int index) {
	if (index == 0) {
		maPlatformRequest("http://www.mobidex.biz/terms");
	}
	else if (index == 1) {
		termsBox->setChecked(true);
		this->show();
	}
	else if (index == 2) {
		termsBox->setChecked(false);
		this->show();
	}
}
