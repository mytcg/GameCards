#include "Deck.h"
#include "Util.h"

Deck::Deck() {
	id = "";
	description = "";
	type="";
	active="";
}

Deck::Deck(String id, String desc, String type, String active):id(id), description(desc),
		type(type), active(active) {
}

Deck::~Deck() {
	id = "";
	description = "";
	type="";
	active="";
}

String Deck::getId() {
	return id;
}

void Deck::setId(const char *i) {
	id = i;
}

String Deck::getType() {
	return type;
}

void Deck::setType(const char *t) {
	type = t;
}

String Deck::getActive() {
	return active;
}

void Deck::setActive(const char *a) {
	active = a;
}

String Deck::getDescription() {
	return description;
}

void Deck::setDescription(const char *d) {
	description = d;
}
