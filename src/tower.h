#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "geometry.h"

class Tower {
 public:
  Position m_position_tl;
  int m_size_tl;
  SDL_Texture* m_texture;

  Tower(Position position_tl, int shape_tl, SDL_Texture* texture);
  void render(Dimension tileshape, SDL_Renderer* renderer);
  bool operator<(const Tower& other) const;
  Position get_position(Dimension tileshape);
};

void delete_tower_at(Position position, std::vector<Tower>* towers);
bool can_put_tower_here(Position position_tl, std::vector<Tower>* towers);
bool line_passes_through_tower(Position start,
                               Position end,
                               std::vector<Tower>* towers,
                               Dimension tower_shape);

void get_tower_borders(Tower tower,
                       Dimension tower_shape,
                       int* return_small_x,
                       int* return_big_x,
                       int* return_small_y,
                       int* return_big_y);
