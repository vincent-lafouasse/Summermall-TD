#pragma once

#include <SDL2/SDL.h>
#include "geometry.h"

class Tower {
 public:
  Position m_position;
  Dimension m_shape;
  SDL_Texture* m_texture;

  Tower(Position position, Dimension shape, SDL_Texture* texture);
  void render(SDL_Renderer* renderer);
};

bool can_put_tower_here(Position position,
                        std::vector<Tower>* towers,
                        Dimension tower_shape);
