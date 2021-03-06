#include "Card.h"
#include "Util.h"

Card::Card() {
	quantity = "";
	text = "";
	thumb = "";
	front = "";
	back = "";
	id = "";
	usercardid = "";
	categoryid = "";
	categoryaddonid = "";
	deckid = "";
	rate = "";
	value = "";
	fullDesc = "";
	note = "";
	frontflip = "";
	backflip = "";
	ranking = "";
	rarity = "";
	position = "";
	positionid = "";
	points = "";
	loaded = false;
	updated = false;
}

Card::~Card() {
	quantity = "";
	text = "";
	thumb = "";
	front = "";
	back = "";
	id = "";
	usercardid = "";
	categoryid = "";
	categoryaddonid = "";
	deckid = "";
	rate = "";
	value = "";
	frontflip = "";
	backflip = "";
	ranking = "";
	rarity = "";
	gamePlayerCardId="";
	fullDesc = "";
	note = "";
	position = "";
	positionid = "";
	points = "";

	int size = stats.size();
	for (int i = 0; i < size; i++) {
		if (stats[i] != NULL) {
			delete stats[i];
			stats[i] = NULL;
		}
	}
	stats.clear();
}

void Card::setLoaded(bool load) {
	loaded = load;
}

bool Card::getLoaded() {
	return loaded;
}

void Card::setUpdated(bool u) {
	updated = u;
}

bool Card::getUpdated() {
	return updated;
}

String Card::getQuantity() {
	return quantity;
}
void Card::setQuantity(const char *quan) {
	quantity = quan;
}

String Card::getText() {
	return text;
}
void Card::setText(const char *tex) {
	text = tex;
}

String Card::getSlotDescription() {
	return slotdescription;
}
void Card::setSlotDescription(const char *desc) {
	slotdescription = desc;
}

void Card::setCategoryId(const char *catid) {
	categoryid = catid;
}
String Card::getCategoryId() {
	return categoryid;
}

void Card::setCategoryAddonId(const char *cataddid) {
	categoryaddonid = cataddid;
}
String Card::getCategoryAddonId() {
	return categoryaddonid;
}

void Card::setDeckId(const char *catid) {
	deckid = catid;
}
String Card::getDeckId() {
	return deckid;
}

String Card::getThumb() {
	return thumb;
}

void Card::setThumb(const char *thum) {
	thumb = thum;
}

String Card::getFront() {
	return front;
}

void Card::setFront(const char *fron) {
	front = fron;
}

String Card::getFrontFlip() {
	return frontflip;
}

void Card::setFrontFlip(const char *fron) {
	frontflip = fron;
}

String Card::getBack() {
	return back;
}

void Card::setBack(const char *bac) {
	back = bac;
}

String Card::getBackFlip() {
	return backflip;
}

void Card::setBackFlip(const char *bac) {
	backflip = bac;
}

String Card::getId() {
	return id;
}

void Card::setId(const char *i) {
	id = i;
}

String Card::getPosition() {
	return position;
}

void Card::setPosition(const char *p) {
	position = p;
}

String Card::getPositionId() {
	return positionid;
}

void Card::setPositionId(const char *pi) {
	positionid = pi;
}

String Card::getPoints() {
	return points;
}

void Card::setPoints(const char *poi) {
	points = poi;
}

String Card::getUserCardId() {
	return usercardid;
}

void Card::setUserCardId(const char *uci) {
	usercardid = uci;
}

String Card::getRate() {
	return rate;
}

void Card::setRate(const char *rat) {
	rate = rat;
}

String Card::getRanking() {
	return ranking;
}

void Card::setRanking(const char *rat) {
	ranking = rat;
}

String Card::getRarity() {
	return rarity;
}

void Card::setRarity(const char *rat) {
	rarity = rat;
}

String Card::getValue() {
	return value;
}

void Card::setValue(const char *valu) {
	value = valu;
}

String Card::getFullDesc() {
	return fullDesc;
}

void Card::setFullDesc(const char *f) {
	fullDesc = f;
}

String Card::getGamePlayerCardId() {
	return gamePlayerCardId;
}

void Card::setGamePlayerCardId(const char *g) {
	gamePlayerCardId = g;
}

String Card::getNote() {
	return note;
}

void Card::setNote(const char *n) {
	note = n;
}

String Card::getAll() {
	String all = getQuantity()+","+getText()+","+getThumb()+","+getFront()+
			","+getBack()+","+getId()+","+getRate()+","+getValue()+","+getNote()+","+
			getRanking()+","+getRarity()+","+getFrontFlip()+","+getBackFlip()+",";
	for (int i = 0; i < stats.size(); i++) {
		all += stats[i]->getAll() + "$";
	}
	return all;
}

void Card::setAll(const char* allch) {
		String all = allch;
		int indexof = all.find(",");
		if (indexof > -1) {
			String sub = all.substr(0,indexof++);
			setQuantity(sub.c_str());
			all=all.substr(indexof);
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setText(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setThumb(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setFront(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setBack(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setId(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setRate(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setValue(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setNote(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setRanking(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setRarity(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setFrontFlip(sub.c_str());
				all=all.substr(indexof);
			}
			indexof = all.find(",");
			if (indexof > -1) {
				sub = all.substr(0,indexof++);
				setBackFlip(sub.c_str());
				all=all.substr(indexof);
			}
			Stat *newStat;
			while (all.length() > 1) {
				indexof = all.find("$");
				if (indexof > -1) {
					newStat = new Stat();
					sub = all.substr(0,indexof++);
					newStat->setAll(sub.c_str());
					stats.add(newStat);
					all=all.substr(indexof);
				}
			}

			setLoaded(true);
			if (getPosition().length() == 0 && ((getText().length() <= 0)||(getQuantity().length() <= 0))) {
				setQuantity("");
				setText("");
				setLoaded(false);
			}
			sub = "";
		} else {
			setQuantity("");
			setText("");
			setThumb("");
			setFront("");
			setBack("");
			setId("");
			setRate("");
			setValue("");
			setNote("");
			setRarity("");
			setRanking("");
			setBackFlip("");
			setFrontFlip("");
			setLoaded(false);
		}
		all = "";
		allch = "";
}

void Card::addStat(Stat *stat) {
	stats.add(stat);
}

void Card::removeStatAt(int index) {
	if (index < stats.size()) {
		stats.remove(index);
	}
}

void Card::setStats(Vector<Stat*> s) {
	stats.clear();
	for (int i = 0; i < s.size(); i++) {
		stats.add(s[i]);
	}
}

Vector<Stat*> Card::getStats() {
	return stats;
}

Stat* Card::getStatAt(int index) {
	return stats[index];
}
