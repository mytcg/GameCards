#include "Stat.h"
#include "Util.h"

Stat::Stat() {
	desc = "";
	display = "";
	iVal = "";
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
	return getDesc()+delim+getDisplay()+delim+getIVal();
}

void Stat::setAll(const char* allch) {
	String all = allch;
	int indexof = all.find(delim);
	if (indexof > -1) {
		setDesc(all.substr(0,indexof++).c_str());
		all=all.substr(indexof);
		indexof = all.find(delim);
		setDisplay(all.substr(0,indexof++).c_str());
		all=all.substr(indexof);
		indexof = all.find(delim);
		setIVal(all.substr(0,indexof++).c_str());
	} else {
		setDesc("");
		setDisplay("");
		setIVal("");
	}
}
