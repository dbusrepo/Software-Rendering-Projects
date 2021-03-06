#ifndef VIS_PLANE_H
#define VIS_PLANE_H

#include "Sector.h"

class VisPlane {
public:
  
  VisPlane(int n_spans, Sector *vis_plane_sector, VisPlane *vis_plane_next)
  : owner(vis_plane_sector), next(vis_plane_next)
  { 
    xmin = 100000;
    xmax = -1;
    ymin = 100000;
    ymax = -1;
    spans = new Span[n_spans];
  }
  
  ~VisPlane() { delete[] spans; }
  
  struct Span {
    int y1, y2;
  } *spans;
  
  int xmin, xmax;
  int ymin, ymax;
  
  Sector *owner;  
  VisPlane *next;
};

#endif