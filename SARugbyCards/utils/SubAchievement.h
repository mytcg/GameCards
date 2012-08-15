#ifndef _SUBACHIEVEMENT_H_
#define _SUBACHIEVEMENT_H_

#include <MAUtil/String.h>

using namespace MAUtil;

class SubAchievement  {
public:
	SubAchievement(int progress, int target, String completedImage, String dateCompleted);
	~SubAchievement();

	void setProgress(int progress);
	int getProgress();
	void setTarget(int target);
	int getTarget();

	void setCompletedImage(String comepletedImage);
	String getCompletedImage();
	void setDateCompleted(String dateCompleted);
	String getDateCompleted();

private:
	int progress;
	int target;
	String completeImage;
	String dateCompleted;
};

#endif	//_SUBACHIEVEMENT_H_
