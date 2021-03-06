#include "Stat.h"
#include "Util.h"

Stat::Stat() {
	desc = "";
	display = "";
	iVal = "";
	cardStatId="";
	categoryStatId="";
	colorRed = 0;
	colorGreen = 0;
	colorBlue = 0;
}

Stat::~Stat() {
	desc = "";
	display = "";
	iVal = "";
	cardStatId="";
	categoryStatId="";
}

void Stat::setCardStatId(const char *d) {
	cardStatId = d;
}

String Stat::getCardStatId() {
	return cardStatId;
}

void Stat::setDesc(const char *d) {
	desc = d;
}

String Stat::getDesc() {
	return desc;
}

void Stat::setDisplay(const char *d) {
	display = d;
}

String Stat::getDisplay() {
	return display;
}

void Stat::setIVal(const char *i) {
	iVal = i;
}

String Stat::getIVal() {
	return iVal;
}

String Stat::getAll() {
	return getDesc()+","+getDisplay()+","+getIVal();
}

void Stat::setCategoryStatId(const char *c) {
	categoryStatId = c;
}

String Stat::getCategoryStatId() {
	return categoryStatId;
}

void Stat::setAll(const char* allch) {
	String all = allch;
	int indexof = all.find(",");
	if (indexof > -1) {
		String sub = all.substr(0,indexof++);
		setDesc(sub.c_str());
		all=all.substr(indexof);
		indexof = all.find(",");
		sub = all.substr(0,indexof++);
		setDisplay(sub.c_str());
		all=all.substr(indexof);
		indexof = all.find(",");
		sub = all.substr(0,indexof++);
		setIVal(sub.c_str());
		sub = "";
	} else {
		setDesc("");
		setDisplay("");
		setIVal("");
	}
	all = "";
}

void Stat::setTop(int i) {
	top = i;
}

int Stat::getTop() {
	return top;
}

void Stat::setLeft(int i) {
	left = i;
}

int Stat::getLeft() {
	return left;
}

void Stat::setWidth(int i) {
	width = i;
}

int Stat::getWidth() {
	return width;
}

void Stat::setHeight(int i) {
	height = i;
}

int Stat::getHeight() {
	return height;
}

void Stat::setFrontOrBack(int i) {
	frontOrBack = i;
}

int Stat::getFrontOrBack() {
	return frontOrBack;
}

void Stat::setColorRed(int i) {
	colorRed = i;
}

int Stat::getColorRed() {
	return colorRed;
}

void Stat::setColorGreen(int i) {
	colorGreen = i;
}

int Stat::getColorGreen() {
	return colorGreen;
}

void Stat::setColorBlue(int i) {
	colorBlue = i;
}

int Stat::getColorBlue() {
	return colorBlue;
}

void Stat::setSelectable(int i) {
	selectable = i;
}

int Stat::getSelectable() {
	return selectable;
}
