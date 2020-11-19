#ifndef XTEXTURE_LOADER_H
#define XTEXTURE_LOADER_H

#include "../src/XScreenRgbInfo.h"

class XTextureLoader
{
public:

	XTextureLoader(XScreenRgbInfo *scr_info)
	{
		this->si = scr_info;
		data = 0;
	}

	virtual ~XTextureLoader()
	{
	}

	virtual void load(const char *fname) = 0;

	XScreenRgbInfo *si;
	unsigned char *data;
	int width, height;
};

#endif
