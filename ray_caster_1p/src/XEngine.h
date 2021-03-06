#ifndef ENGINE_H
#define ENGINE_H

#include "XScreenRgbInfo.h"
#include "XRasterizer.h"
#include "XTextureLoader.h"

#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

class XEngine {
public:
  
  XEngine(int xsize, int ysize, XScreenRgbInfo *scr_info);
  
  void draw_point(int x, int y, unsigned long col) {
     rasterizer->draw_point(x, y, col);
  }
  
  void draw_line(int x0, int y0, int x1, int y1, unsigned long col) {
    rasterizer->draw_line(x0, y0, x1, y1, col);
  }
  
  void clear_screen() {
    rasterizer->clear_buffer();
  }
  
  void render_screen() {
    render_view();
    render_floor();
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
  unsigned int *p_screen;
  int xsize, ysize;
  int vp_left, vp_right, vp_top, vp_bot, vp_center;
  int xview, yview;
  

  XTextureLoader *tex_loader;
  unsigned int ***x_wall_textures;
  unsigned int ***y_wall_textures;
  unsigned int ***floor_textures;
  unsigned int ***ceiling_textures;
  unsigned int *sky_tex;
  
  int light_distance_levels;
  int *light_distance;
  float *distances;
  int *top_max;
    
  struct plane {
    int code;
    int tex;
    int wall_tex;
    unsigned int height;
  } *floor;
  
  struct plane_render_data {
    float ratio;
    float distance;
    int light_level;
    int x, y;
    int dx, dy;
  } render_row[100];
  
  const int map_size;
  
  int xp, yp, height_p, pp_distance;
  int cur_height_p;
  int xd, yd;
  float angle_p, angle_d;
  float cos_angle_p;
  float sin_angle_p;
  
  float max_distance;

  const int fov;
  struct col_inf {
    float ang, cos_ang, sin_ang;
  } *col_data;
  

  void init_map();
  void init_data(); 
  void setup_textures();
  void create_sky_texture();
  void gen_light_textures(unsigned int ***, int, const char *);
  void compute_wall_tex_offsets_table();
  void setup_screen_data();
  double deg_to_rad(int deg);
  void render_view();
  void render_floor();
  
};

#endif