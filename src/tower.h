#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "geometry.h"

class Tower {
 public:
  Position m_position;
  Dimension m_shape;
  SDL_Texture* m_texture;

  Tower(Position position, Dimension shape, SDL_Texture* texture);
  void render(SDL_Renderer* renderer);
  bool operator<(const Tower& other) const;
};

void delete_tower_at(Position position, std::vector<Tower>* towers);
bool can_put_tower_here(Position position,
                        std::vector<Tower>* towers,
                        Dimension tower_shape);
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
