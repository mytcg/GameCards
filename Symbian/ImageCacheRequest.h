/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef IMAGECACHEREQUEST_H_
#define IMAGECACHEREQUEST_H_

#include <MAUtil/String.h>
#include <ma.h>
#include <MAUI/Image.h>
#include "Card.h"


using namespace MAUtil;
using namespace MAUI;

class ImageCacheRequest
{
  public:
    ImageCacheRequest();
    ImageCacheRequest(Image *img, Card *card, int height, int type);
    ~ImageCacheRequest();

    String getUrl();
    String getSaveName();
    Image* getImage();
    int getHeight();
    int getType();

  private:
    Card *card;
    Image *img;
	int height, type;
};



#endif /* IMAGECACHEREQUEST_H_ */
