#include <cmath>
#include "Wall.h"

Wall::Wall(int wall_id, const Vector2d& p1, const Vector2d& p2, unsigned int wall_color, Texture *wall_texture)
: id(wall_id), color(wall_color), tex(wall_texture), next(0), next_sort(0), type(Wall::SOLID)
{
  points[0] = p1;
  points[1] = p2;
  double dx = p2.x - p1.x; 
  double dy = p2.y - p1.y;
  ang = atan2(dy, dx);
  compute_tan();
  compute_length();
  owner_plane = new Plane(*this);
  tex_voffset = 0;
  tex_hoffset = 0;
} 

void Wall::compute_tan() {
  double dx = points[1].x - points[0].x; 
  double dy = points[1].y - points[0].y;  
  tan_ang = dy / dx;
}
  
void Wall::compute_length() {
  double dx = points[1].x - points[0].x; 
  double dy = points[1].y - points[0].y;
  length = sqrt(dx*dx + dy*dy);
}
  