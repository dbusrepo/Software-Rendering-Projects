#ifndef VECTOR_2D_H
#define VECTOR_2D_H

class Vector2d {
public:
  Vector2d() {}
  Vector2d(int dx, int dy) : x(dx), y(dy) {}
  int x, y;
};

#endif