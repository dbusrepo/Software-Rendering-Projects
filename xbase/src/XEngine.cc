#include "XEngine.h"

#include "XTextureLoaderPPM.h"

XEngine::XEngine(int xsize, int ysize, XScreenRgbInfo *scr_info) {
  this->scr_info = scr_info;
  this->rasterizer = new XRasterizer(xsize, ysize, scr_info->p_screenbuf, scr_info->bytes_per_pixel);
  this->tex_loader = new XTextureLoaderPPM(scr_info);
}