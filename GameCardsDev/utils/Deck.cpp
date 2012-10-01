#include "Deck.h"
#include "Util.h"

Deck::Deck() {
	id = "";
	description = "";
	type="";
}

Deck::Deck(String id, String desc, String type):id(id), description(desc),
		type(type) {
}

Deck::~Deck() {
	id = "";
	description = "";
	type="";
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

String Deck::getDescription() {
	return description;
}

void Deck::setDescription(const char *d) {
	description = d;
}
