#ifndef ENGINE_H
#define ENGINE_H

#include "XScreenRgbInfo.h"
#include "XRasterizer.h"
#include "XTextureLoader.h"

#include "Player.h"
#include "VertexSet.h"
#include "BspTree.h"
#include "VisPlane.h"


#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

class XEngine {
public:
  
  XEngine(int xsize, int ysize, XScreenRgbInfo *scr_info);
 
  void clear_screen() {
    rasterizer->clear_buffer();
  }
  
  void render_screen() {
    render();
  }
  
  struct KeyPressed {
    KeyPressed() {
      moving_backward = moving_forward = turning_left = turning_right = false;
    }
    bool moving_forward;
    bool moving_backward;
    bool turning_left;
    bool turning_right; 
  } key_pressed;
  void process_input();
  virtual ~XEngine() { }
  
private:
  XScreenRgbInfo *scr_info;
  XRasterizer *rasterizer;
  XTextureLoader *tex_loader;
  
  unsigned int *p_screen, **row_to_pscreen;
  int xsize, ysize;
  int vp_left, vp_right, vp_top, vp_bot, vp_center;
  int xview;
  int pp_distance;
  double *floor_ratio, *ceiling_ratio;
  
  const int fov;
  struct col_info {
    double ang, cos_ang, sin_ang;
  } *col_data;
    
  
  Texture *wall_tex[3];
  Texture *floor_tex[3];
  Texture *ceiling_tex[3];
  Player *player;
  double cos_ang_p;
  double sin_ang_p;
  int cur_height;
  bool cur_height_set;

  
  VertexSet *vertex_set;
  Vector2d *t_vlist;
  BspTree *bsp_tree;  
  int *z_lower, *z_upper;
  
  struct screen_rendering {
    bool stop;
    bool *clip;
    int n_col; 
    int y_min;
    int y_max;
    int x_min;
    int x_max;
    struct wall_span {
	int ytop, ybot;
	unsigned int *tex_col_ptr;
	int tex_off;
	int tex_inc;
	int tex_height;
    } *screen_spans;
    
    struct lu_wall {
      lu_wall(int xsize, lu_wall *wnext)
      : next(wnext) {
	luwall_spans = new wall_span[xsize];
	y_min = 10000;
	y_max = -1;
	x_min = 10000;
	x_max = -1;
      }
      
      ~lu_wall() { delete [] luwall_spans; }
      
      wall_span *luwall_spans;
      int y_min;
      int y_max;
      int x_min;
      int x_max;
      lu_wall *next;
    } *l_walls_list, *u_walls_list;
    
    VisPlane *floor_planes_list;
    VisPlane *ceiling_planes_list;
  } render_info;
    
  void setup_data(); 
  void setup_level_data();
  void setup_screen_data();
  void setup_player();
  void setup_textures();
  double deg_to_rad(int deg);
  void render();
  void process_solid_wall(const Wall& wall); 
  void process_portal_wall(const Wall& wall, Wall::SideTypes side); 
  void render_solid_walls();
  void render_floor();
  void render_ceiling();
  bool visible_wall(Wall &wall, Wall::SideTypes cur_side, Wall::SideTypes next_side);
  bool visible_bwall(Wall &wall);
  void compute_visible_walls();
  void compute_visible_walls_rec(BspTree::BspNode *pnode, const Vector2d& pos);
  void process_wall(Plane::Position p, Wall *);
  bool brect_visible(BspTree::BspNode *pnode);
  void init_tvlist();
  void init_zBuffers();
  void init_render();
  void update_tvlist();
    
  void update_position(double d);    
  void update_angle(double d); 
  void update_height(int d); 
  void update_vp(int d);  
  
  Wall *create_square_room(Texture *tex, Sector *sector, const Vector2d& pos, int edge_length, Wall *wall_list);
  Wall *create_column(Texture *tex, Sector *sector, const Vector2d& pos, int edge_length, Wall *wall_list);
  
};

#endif