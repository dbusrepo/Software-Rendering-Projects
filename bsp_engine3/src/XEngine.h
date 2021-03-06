#ifndef ENGINE_H
#define ENGINE_H

#include "XScreenRgbInfo.h"
#include "XRasterizer.h"
#include "XTextureLoader.h"

#include "ScreenRender.h"
#include "Player.h"
#include "VertexSet.h"
#include "BspTree.h"
#include "VisPlane.h"
#include "Door.h"
#include "Elevator.h"

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
  
  ScreenRender *render_info;
     
  Texture *wall_tex[3];
  Texture *floor_tex[3];
  Texture *ceiling_tex[3];
  Player *player;
    
  Sector *sector[MAX_NSECTORS];
  int num_sectors;
  
  VertexSet *vertex_set;
  Vector2d *t_vlist;
  BspTree *bsp_tree;  
  Sector *vis_sectors_list;
  Sector **vis_sectors_list_ptr;
  Door *doors_list, *active_doors_list;
  Elevator *elevs_list, *active_elevs_list;
  
  void init_sectors_data();
  void build_sec_adj_list(Sector *s);
  
  void init_doors_data();
  Door *find_door_sector(Sector *sec);
  bool door_collision(Wall *w);
  void update_doors_list();
  bool can_close_door(Door *d);
  void insert_door_in_adl(Door *d);
  void remove_door_from_adl(Door *d);
  
  void init_elevators_data();
  Elevator *find_elev_sector(Sector *sec);
  void update_elevs_list();
  void insert_elev_in_ael(Elevator *d);
  void remove_elev_from_ael(Elevator *d);
  void check_elev_activation();
      
  void setup_data(); 
  void setup_level_data();
  void setup_lists();
  void setup_player();
  void setup_textures();
  void render();
  void process_solid_wall(const Wall& wall); 
  void process_portal_wall(const Wall& wall, Wall::SideTypes side); 
  bool visible_wall(Wall &wall, Wall::SideTypes cur_side, Wall::SideTypes next_side);
  bool visible_bwall(Wall &wall);
  void compute_visible_walls();
  void compute_visible_walls_rec(BspTree::BspNode *pnode);
  void process_wall(Plane::Position p, Wall *);
  bool brect_visible(BspTree::BspNode *pnode);
  void init();
  void update();
  bool collision(const Wall& wall, const Vector2d& point);
  bool check_collisions(const Vector2d& point);
  void insert_sec_in_vis_list(Sector *);
  void print_sectors_list();
  void compute_max_ceiling_height(Sector *s);
    
  void update_position(double d);    
  void update_angle(double d); 
  void update_height(double d); 

  Wall *create_square_room(Texture *tex, Sector *sector, const Vector2d& pos, int edge_length, Wall *wall_list);
  Wall *create_column(Texture *tex, Sector *sector, const Vector2d& pos, int edge_length, Wall *wall_list);
  Wall *create_random_walls_level();
};

#endif