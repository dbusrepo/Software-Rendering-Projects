#ifndef BSP_TREE_H
#define BSP_TREE_H

#include "Plane.h"
#include "VertexSet.h"

class Plane;

class BspTree {
public:
  
  BspTree(Wall *wall_list, VertexSet *vertex_set_ptr);
  
  struct BspNode {
    BspNode() : partition(0), walls(0), back(0), front(0) { brect[0]=brect[1]=brect[2]=brect[3]=0; }
    void build_brect(Wall *wall_list, VertexSet *vertex_set_ptr);
    Plane *partition;
    Wall *brect[4];
    Wall *walls;
    BspNode *front, *back;
  };
  
  BspNode *root;
  int n_nodes, n_poly;
  VertexSet *vertex_set;
  int s_walls_id, p_walls_id;
  
  void print();
  void print_rec(BspNode *pnode, int spaces);
private:
  void build(BspNode *pnode, Wall *wall_list);
  Wall *reorganize(Wall *wall_list);
  unsigned int count_split(Wall *wall, Wall *wall_list);
  Wall::CoPlane_Side decide_side(Plane *p, Wall *w);
  void init_walls(Wall *wall_list);
};

#endif