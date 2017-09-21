#ifndef WALL_SPAN_H
#define WALL_SPAN_H


class WallSpan {
public:
  short ytop, ybot;
  unsigned int *tex_col_ptr;
  int tex_off;
  int tex_inc;
  int tex_height;	
};

#endif