#include <cmath>
#include "Plane.h"
#include "com_def.h"

#include <iostream>
using std::cout;
using std::endl;

const double Plane::EPS = 0.0001;

Plane::Plane(const Wall& wall) {
  double x1 = wall.points[0]->x;
  double y1 = wall.points[0]->y;
  double x2 = wall.points[1]->x;
  double y2 = wall.points[1]->y;
  double dx = x2 - x1;
  double dy = y2 - y1;
  a = -dy;
  b = dx;
  c = dy * x1 - dx * y1;  
  idd = 1.0/sqrt(a*a + b*b);
}

bool Plane::collision(const Vector2d& point) {
  return fabs(evaluate(point) * idd) < COLLISION_DISTANCE;
}

bool Plane::collision(const Vector2d& point, double distance) {
  return fabs(evaluate(point) * idd) <= distance;
}

Plane::Position Plane::evaluate_position(const Wall& wall) {
  Plane::Position a = evaluate_position(*wall.points[0]);
  Plane::Position b = evaluate_position(*wall.points[1]);
  
  if (a == SPANNING)
    if (b == SPANNING) 
      return COINCIDENT;
    else
      return b;
    
  if (b == SPANNING)
    return a;
  
  if ((a == IN_FRONT && b == IN_BACK) || (a == IN_BACK && b == IN_FRONT))
    return SPANNING;
  
  return a;  
}

// splitta wall rispetto a this, in wall1 si mette la parte in front, in wall2 la parte in back
bool Plane::split(const Wall& wall, Wall& wall1, Wall& wall2, VertexSet& vertex_set) {
  Plane p(wall);
  double cross_x = 0.0f;
  double cross_y = 0.0f;
  double divider = a * p.b - b * p.a;
  
  if (divider == 0) {
    if (p.a == 0)
      cross_x = wall.points[0]->x;
    if (p.b == 0)
      cross_y = wall.points[0]->y;
    if (a == 0)
      cross_y = -b;
    if (b == 0)
      cross_x = c;
  } else {
    cross_x = (-c * p.b + b * p.c) / divider;
    cross_y = (-a * p.c + c * p.a) / divider;
  }
  
  Position p1 = evaluate_position(*wall.points[0]);
  Position p2 = evaluate_position(*wall.points[1]);
  Vector2d *crossp_addr = vertex_set.insert(Vector2d(cross_x, cross_y));
    
  if (p1 == IN_BACK && p2 == IN_FRONT) {  
    wall1.points[0] = crossp_addr;
    wall1.points[1] = wall.points[1];
    wall2.points[0] = wall.points[0];
    wall2.points[1] = crossp_addr;
  } else if (p1 == IN_FRONT && p2 == IN_BACK) {
    wall1.points[0] = wall.points[0];
    wall1.points[1] = crossp_addr;
    wall2.points[0] = crossp_addr;
    wall2.points[1] = wall.points[1];
  } else {
    return false;
  }
  
  wall1.compute_components();
  wall2.compute_components();
  wall1.compute_tan();
  wall1.compute_length(); 
  wall2.compute_tan();
  wall2.compute_length();
    
  return true;
  
}