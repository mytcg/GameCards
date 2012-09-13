#ifndef MOBFONT_H_
#define MOBFONT_H_

#include <MAUI/Font.h>

using namespace MAUtil;
using namespace MAUI;

class MobFont: public Font {
	public:
		/** Constructor, takes a handle to a binary resource with font data **/
		MobFont(MAHandle font);

		/** Renders str at x, y **/
		void drawString(const char* str, int x, int y, int rotation = TRANS_NONE);

		/**
		* Renders str, linebroken according to bounds, at x,y.
		* Adds \a lineSpacing pixels of space between each line.
		*/
		void drawBoundedString(const char* str, int x, int y, const Rect& bound, int rotation = TRANS_NONE);

	protected:
		void calcLineBreaks(const char* strS, int x, int y, const Rect& bound) const;
};

#endif
