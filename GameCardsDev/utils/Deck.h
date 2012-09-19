#ifndef _DECK_H_
#define _DECK_H_

#include <MAUtil/String.h>

using namespace MAUtil;

class Deck  {
public:
	Deck();
	Deck(String id, String desc, String type);
	~Deck();

	String getId();
	void setId(const char *id);

	String getType();
	void setType(const char *type);

	String getDescription();
	void setDescription(const char *description);

private:
	String id;
	String description;
	String type;
};

#endif	//_DECK_H_
