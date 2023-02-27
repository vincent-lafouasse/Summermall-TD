#pragma once

#include <SDL2/SDL.h>
#include "geometry.h"

class Tower {
  Position position;

  void render(SDL_Renderer* renderer);
};

bool can_put_tower_here(Position position, std::vector<Tower>* towers);
