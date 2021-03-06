#ifndef ENGINE_H
#define ENGINE_H

#include "XScreenRgbInfo.h"
#include "XRasterizer.h"
#include "XTextureLoader.h"

#include "Player.h"
#include "BspTree.h"

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
//     render_solid_background();
    render_background();
    render();
  }
  
  void process_key(char ch);
  
  void update_position(int dir);
    
  void update_angle(int dir);
  
  void update_height(int dir);
  
  void update_vp(int dir);
  
  virtual ~XEngine() { }
  
private:
  XScreenRgbInfo *scr_info;
  XRasterizer *rasterizer;
  XTextureLoader *tex_loader;
  
  unsigned int *p_screen;
  int xsize, ysize;
  int vp_left, vp_right, vp_top, vp_bot, vp_center;
  int xview, yview;
  
  int pp_distance;
  
  const int fov;
  struct col_inf {
    double ang, cos_ang, sin_ang;
  } *col_data;
    
  
  Texture *wall_tex, *floor_tex;
  Player *player;
  double cos_ang_p;
  double sin_ang_p;
  int cur_height_p;
  
  BspTree *bsp_tree;
  Wall *wall_list;
  int n_clipped_walls;
  int *z_lower;
  
  void setup_data(); 
  void setup_level_data();
  void setup_screen_data();
  void setup_player();
  void setup_textures();
  double deg_to_rad(int deg);
  void render();
  void render_wall(const Wall& wall); 
  bool visible_wall(Wall &wall);
  bool visible_bwall(Wall &wall);
  void compute_visible_walls();
  void compute_visible_walls_rec(BspTree::BspNode *pnode, const Vector2d& pos);
  bool brect_visible(BspTree::BspNode *pnode);
  void render_solid_background();
  void render_background();
  void init_zLower();
  
};

#endif