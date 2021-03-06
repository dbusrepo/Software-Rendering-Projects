#ifndef LU_WALL_H
#define LU_WALL_H

#include "WallSpan.h"

class LUWall {
public:
  LUWall(int xsize, LUWall *wnext) : next(0) {
    luwall_spans = new WallSpan[xsize];
    setup(wnext);
  }
      
  void setup(LUWall *wnext) {
    y_min = 10000;
    y_max = -1;
    x_min = 10000;
    x_max = -1;	
    next_vis = wnext;
  }
      
  ~LUWall() { 
    delete [] luwall_spans; 
  }
      
  WallSpan *luwall_spans;
  short y_min, y_max;
  short x_min, x_max;
  LUWall *next;
  LUWall *next_vis;
};

#endif