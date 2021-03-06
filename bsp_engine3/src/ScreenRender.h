#ifndef SCREEN_RENDERING_H
#define SCREEN_RENDERING_H

#include <cstring>
#include "WallSpan.h"
#include "LUWall.h"
#include "VisPlane.h"
#include "Player.h"
#include "com_def.h"

class ScreenRender {
public:
  ScreenRender(unsigned char *scr_buffer, int scr_xsize, int scr_ysize);
  
  void render(Player *player) {
    render_vis_planes(player);
    render_solid_walls();    
  }
  
  void reset_render_data() {
    stop = false;
    n_col = xview;
    memcpy(z_lower, z_mem_init, 2 * sizeof(int) * xsize);
    memset(clip, 0, (1 + xsize) * sizeof(int));
    for (int i=0; i!=xsize; ++i)
      wall_spans[i].ytop = wall_spans[i].ybot = 0xFFFF;
    y_min = ysize+1;
    y_max = -1;
    x_min = xsize+1;
    x_max = -1;
    l_walls_list = 0;
    u_walls_list = 0;
    d_walls_list = 0;
    e_walls_list = 0;
    floor_planes_list = 0;
    ceiling_planes_list = 0;    
  }
  
  void release_used_list() {
    release_used_vis_planes_list();
    release_used_lu_walls_list();
  }
    
  VisPlane *get_new_vis_plane(Sector *vis_plane_sector, VisPlane *vis_plane_next) {
  
    VisPlane *vis_plane_ptr;
  
    if (free_planes_list) {
      vis_plane_ptr = free_planes_list;
      free_planes_list = free_planes_list->next;
      vis_plane_ptr->setup(vis_plane_sector, vis_plane_next);
    } else {
      vis_plane_ptr = new VisPlane(xsize, vis_plane_sector, vis_plane_next);
    }
  
    vis_plane_ptr->next = used_planes_list;  
    if (!used_planes_list)
      last_used_plane = vis_plane_ptr;
    used_planes_list = vis_plane_ptr;
  
    return vis_plane_ptr;
  }

  LUWall *get_new_lu_wall(LUWall *lu_wall_next) {   
    LUWall *lu_wall_ptr;
    
    if (free_lu_walls_list) {
      lu_wall_ptr = free_lu_walls_list;
      free_lu_walls_list = free_lu_walls_list->next;
      lu_wall_ptr->setup(lu_wall_next);
    } else {
      lu_wall_ptr = new LUWall(xsize, lu_wall_next);
    }
    
    lu_wall_ptr->next = used_lu_walls_list;
    if (!used_lu_walls_list)
      last_used_lu_wall = lu_wall_ptr;
    used_lu_walls_list = lu_wall_ptr;
    
    return lu_wall_ptr;
  } 
  
  void release_used_vis_planes_list() {
    if (used_planes_list) {
      last_used_plane->next = free_planes_list;
      free_planes_list = used_planes_list;
      used_planes_list = last_used_plane = 0;
    }
  }  
  
  void release_used_lu_walls_list() {
    if (used_lu_walls_list) {
      last_used_lu_wall->next = free_lu_walls_list;
      free_lu_walls_list = used_lu_walls_list;
      used_lu_walls_list = last_used_lu_wall = 0;
    }
  }

  // vp_left <= x1 <= x2 <= vp_right
  bool wall_occluded(int x1, int x2) {  
    if (clip[x1] != 0 && (clip[x1] == clip[x2])) {
      return true;
    }
    int tmp = clip[x2+1];
    clip[x2+1] = 0;
    while (clip[x1])
      ++x1;
    clip[x2+1] = tmp;	
    return x1 > x2;
  }
  
  
  double deg_to_rad(int deg);
  void render_solid_walls();
  void render_vis_planes(Player *player);
  void compute_floor_ceiling_factors(Player *player);
  void draw_hspan(unsigned  int *screen_ptr, unsigned int *limit, 
		  unsigned int *tex_ptr, int fxl, int fyl, int fdx, int fdy);
      
  int xsize, ysize;
  int hxsize;
  int xview;
  
  unsigned int *p_screen, **row_to_pscreen;
  int vp_left, vp_right, vp_top, vp_bot, vp_center;
  double inv_width;
  int pp_distance;
  double *floor_ratio, *ceiling_ratio;
  int *z_mem_init;
  int fov;
  struct col_info {
    double ang, cos_ang, sin_ang;
  } *col_data;
  int *z_lower, *z_upper;
  double cos_left_column_angle;
  double sin_left_column_angle;
  
  bool stop;
  int *clip;
  short n_col; 
  short y_min, y_max;
  short x_min, x_max;
    
  WallSpan *wall_spans;
  
  LUWall *l_walls_list, *u_walls_list, *d_walls_list, *e_walls_list;
  LUWall *free_lu_walls_list, *used_lu_walls_list, *last_used_lu_wall;
  
  VisPlane *floor_planes_list;
  VisPlane *ceiling_planes_list;
  VisPlane *free_planes_list, *used_planes_list, *last_used_plane;
  
  double cos_prod, sin_prod;
  double sin_cos, cos_sin;
  double trig_f1, trig_f2, trig_f3, trig_f4;
  
  
};

#endif