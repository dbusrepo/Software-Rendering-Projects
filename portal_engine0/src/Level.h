#ifndef LEVEL_H
#define LEVEL_H

#include "Sector.h"
#include "Player.h"

class Level {
public:
  
  Level(int level_num_sectors, Sector **level_sectors) 
	: num_sectors(level_num_sectors), sectors(level_sectors)
  {
    for (int i=0; i!=num_sectors; ++i) {
       sectors[i]->sort_walls();
    }
  }
    
  int num_sectors;
  Sector **sectors;
  
  Sector *player_sector(const Player *player);   
  
private:
  bool test_in_sector(const Sector *sector, const Player *player);
  bool test_intersection(const Player *player, const Wall *wall);
};

#endif