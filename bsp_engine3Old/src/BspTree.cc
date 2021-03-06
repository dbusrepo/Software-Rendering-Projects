#include "BspTree.h"
#include <cstdlib>
#include <limits>

using std::cout;
using std::endl;

#define MAX_DOUBLE std::numeric_limits<double>::max()

// wall_list != 0
BspTree::BspTree(Wall *wall_list, VertexSet *vertex_set_ptr) : n_nodes(0), n_poly(0) {
  vertex_set = vertex_set_ptr;
  root = new BspTree::BspNode;
  build(root, wall_list);
}
  
void BspTree::BspNode::build_brect(Wall *wall_list, VertexSet *vertex_set_ptr) {
  double xmin, ymin;
  double xmax, ymax;
  
  xmin = MAX_DOUBLE;
  ymin = MAX_DOUBLE;
  xmax = -MAX_DOUBLE;
  ymax = -MAX_DOUBLE;
  
  for (Wall *pwall = wall_list; pwall; pwall = pwall->next) {
    
    if (pwall->points[0]->x < pwall->points[1]->x) {
      if (pwall->points[0]->x < xmin) 
	xmin = pwall->points[0]->x;
      if (pwall->points[1]->x > xmax)
	xmax = pwall->points[1]->x;
    } else {
      if (pwall->points[1]->x < xmin)
	xmin = pwall->points[1]->x;
      if (pwall->points[0]->x > xmax)
	xmax = pwall->points[0]->x;
    }
    
    if (pwall->points[0]->y < pwall->points[1]->y) {
      if (pwall->points[0]->y < ymin) 
	ymin = pwall->points[0]->y;
      if (pwall->points[1]->y > ymax)
	ymax = pwall->points[1]->y;
    } else {
      if (pwall->points[1]->y < ymin)
	ymin = pwall->points[1]->y;
      if (pwall->points[0]->y > ymax)
	ymax = pwall->points[0]->y;
    }    
    
  }
  
  Vector2d *p_addr[4];
    
  p_addr[0] = vertex_set_ptr->insert(Vector2d(xmin, ymin));
  p_addr[1] = vertex_set_ptr->insert(Vector2d(xmax, ymin));
  p_addr[2] = vertex_set_ptr->insert(Vector2d(xmax, ymax));
  p_addr[3] = vertex_set_ptr->insert(Vector2d(xmin, ymax));
  
  brect[0] = new Wall(-1, p_addr[0], p_addr[1], Wall::SOLID, 0, 0, 0, 0, 0, 0, 0, 0);
  brect[1] = new Wall(-1, p_addr[1], p_addr[2], Wall::SOLID, 0, 0, 0, 0, 0, 0, 0, 0);
  brect[2] = new Wall(-1, p_addr[2], p_addr[3], Wall::SOLID, 0, 0, 0, 0, 0, 0, 0, 0);
  brect[3] = new Wall(-1, p_addr[3], p_addr[0], Wall::SOLID, 0, 0, 0, 0, 0, 0, 0, 0);
 
}

// pnode != 0, wall_list != 0
void BspTree::build(BspNode *pnode, Wall *wall_list) {
  ++n_nodes;++n_poly;
  pnode->build_brect(wall_list, vertex_set);
  Wall *wall = wall_list;
  wall_list = wall_list->next;
  pnode->partition = new Plane(*wall);
  wall->next = pnode->walls;
  pnode->walls = wall;
  Wall *front_list = 0;
  Wall *back_list = 0;
  while (wall_list) {
    Wall *cur_wall = wall_list;
    wall_list = wall_list->next;
    Plane::Position pos = pnode->partition->evaluate_position(*cur_wall);
    switch (pos) {
      case Plane::IN_FRONT:
	cur_wall->next = front_list;
	front_list = cur_wall;
	break;
      case Plane::IN_BACK:
	cur_wall->next = back_list;
	back_list = cur_wall;
	break;
      case Plane::SPANNING: {
	Wall *wall1 = new Wall(*cur_wall);
	Wall *wall2 = new Wall(*cur_wall);
	if (!pnode->partition->split(*cur_wall, *wall1, *wall2, *vertex_set))
	  exit(1);		
	
	if (wall1->length > 0) {
	  wall1->next = front_list;
	  front_list = wall1;
	}
	if (wall2->length > 0) {
	  wall2->next = back_list;
	  back_list = wall2;
	}
	
      } break;
      case Plane::COINCIDENT:
	cur_wall->next = pnode->walls;
	pnode->walls = cur_wall;
	++n_poly;
	break;
    }
  }
  
  if (front_list) {
    pnode->front = new BspNode;
    build(pnode->front, front_list);
  }
  
  if (back_list) {
    pnode->back = new BspNode;
    build(pnode->back, back_list);
  }
  
}

void BspTree::print() {
  cout << "------------------------------------------------------------" << endl;
  if (root) 
    print_rec(root, 0);
  cout << "------------------------------------------------------------" << endl; 
}

void BspTree::print_rec(BspNode *pnode, int spaces) {
  if (pnode->front)
    print_rec(pnode->front, spaces+5);
  
  for (int i=0; i!=spaces; ++i)
    cout << " ";
  
  Wall *pwall = pnode->walls;
  while (pwall) {
    cout << pwall->id << " (len: " << pwall->length << ", pos: " << pwall->points[0]->x << " " << pwall->points[0]->y << " " << pwall->points[1]->x << " " << pwall->points[1]->y << ") ";
//     cout << pwall->id << " (" << pwall->length << ") ";
    if (pwall->points[0]->x == 1243) {
      cout << pwall->points[0];
      cout << "***************" << endl;
    }
    
    if (pwall->points[1]->x == 1243) {
      cout << pwall->points[0];
      cout << "***************" << endl;      
    }
    pwall = pwall->next;
  }
  
  cout << endl;
  
  if (pnode->back)
    print_rec(pnode->back, spaces+5);
}

std::ostream& operator<<(std::ostream& oss, const Wall& wall) {
  oss << wall.points[0]->x << " " << wall.points[0]->y << std::endl;
  oss << wall.points[1]->x << " " << wall.points[1]->y << std::endl << std::endl;
  return oss;
}