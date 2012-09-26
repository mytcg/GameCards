#include "SubAchievement.h"

SubAchievement::SubAchievement(int progress, int target, String completedImage, String dateCompleted) :
	progress(progress), target(target), completeImage(completedImage), dateCompleted(dateCompleted) {
}

SubAchievement::~SubAchievement() {
	progress = 0;
	target = 0;
	completeImage = "";
	dateCompleted = "";
}

void SubAchievement::setProgress(int p) {
	progress = p;
}

int SubAchievement::getProgress() {
	return progress;
}

void SubAchievement::setTarget(int t) {
	target = t;
}

int SubAchievement::getTarget() {
	return target;
}

void SubAchievement::setCompletedImage(String c) {
	completeImage = c;
}

String SubAchievement::getCompletedImage() {
	return completeImage;
}

void SubAchievement::setDateCompleted(String d) {
	dateCompleted = d;
}

String SubAchievement::getDateCompleted() {
	return dateCompleted;
}
