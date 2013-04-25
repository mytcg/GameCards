#include "Achievement.h"

Achievement::Achievement() {
	name = "";
	description = "";
	incompleteImage = "";
}

Achievement::Achievement(String name, String description, String incompleteImage) :
	name(name), description(description), incompleteImage(incompleteImage) {
}

Achievement::~Achievement() {
	name = "";
	description = "";
	incompleteImage = "";

	for (int i = 0; i < subAchis.size(); i++) {
		delete subAchis[i];
		subAchis[i] = NULL;
	}
	subAchis.clear();
}

void Achievement::setName(String n) {
	name = n;
}

String Achievement::getName() {
	return name;
}

void Achievement::setDescription(String d) {
	description = d;
}

String Achievement::getDescription() {
	return description;
}

void Achievement::setIncompleteImage(String i) {
	incompleteImage = i;
}

String Achievement::getIncompleteImage() {
	return incompleteImage;
}

String Achievement::getDisplayImage() {
	String retString = incompleteImage;
	for (int i = 0; i < subAchis.size(); i++) {
		if (subAchis[i]->getDateCompleted().length() > 0) {
			retString = subAchis[i]->getCompletedImage();
		}
		else {
			break;
		}
	}
	return retString;
}

SubAchievement* Achievement::getSubAchi(int index) {
	return subAchis[index];
}

void Achievement::addSubAchi(int progress, int target, String completedImage, String dateCompleted) {
	subAchis.add(new SubAchievement(progress, target, completedImage, dateCompleted));
}

int Achievement::subSize() {
	return subAchis.size();
}
