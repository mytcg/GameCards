#ifndef _ACHIEVEMENT_H_
#define _ACHIEVEMENT_H_

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

#include "SubAchievement.h"

using namespace MAUtil;

class Achievement  {
public:
	Achievement();
	Achievement(String name, String description, String incompleteImage);
	~Achievement();

	void setName(String name);
	String getName();
	void setDescription(String description);
	String getDescription();
	void setIncompleteImage(String incompleteImage);
	String getIncompleteImage();

	String getDisplayImage();

	SubAchievement* getSubAchi(int index);
	void addSubAchi(int progress, int target, String completedImage, String dateCompleted);

	int subSize();

private:
	String name;
	String description;
	String incompleteImage;

	Vector<SubAchievement*> subAchis;
};

#endif	//_ACHIEVEMENT_H_
