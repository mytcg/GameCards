#ifndef MOBLABEL_H_
#define MOBLABEL_H_

#include <MAUI/Label.h>

#include "MobFont.h"

using namespace MAUtil;
using namespace MAUI;

class MobLabel: public Label {
	public:
		/** Constructor.
		  * \param x the horizontal position of the Label relative to its parent's top left padded corner.
		  * \param y the vertical position of the Label relative to its parent's top left padded corner
		  * \param width the width of the Label.
		  * \param height the height of the Label.
		  * \param parent pointer to the parent widget. Passing anything else than NULL causes the Label to be added to the parent's children.
		  **/
		MobLabel(int x, int y, int width, int height, Widget* parent, int rotation = TRANS_NONE);
		/** Constructor
		  * \param x the horizontal position of the Label relative to its parent's top left padded corner.
		  * \param y the vertical position of the Label relative to its parent's top left padded corner
		  * \param width the width of the Label.
		  * \param height the height of the Label.
		  * \param parent pointer to the parent widget. Passing anything else than NULL causes the widget to be added to the parent's children.
		  * \param caption the text displayed by the Label.
		  * \param backColor the background color of the widget.
		  * \param font the font to be used by to render the caption.
		  */
		MobLabel(int x, int y, int width, int height, Widget* parent, const String &caption,
			int backColor, Font* font, int rotation = TRANS_NONE);

		void setFont(MobFont* font);

	protected:
		void drawWidget();

	private:
		int rotation;
};

#endif
