#pragma once

#include <SDL2/SDL.h>
#include "geometry.h"

class Monster {
 public:
  Position m_position;
  float m_orientation;

 private:
  SDL_Texture* m_texture;
  Dimension m_shape;

 public:
  Monster(Position position,
          float orientation,
          const Dimension dst_shape,
          SDL_Texture* texture);
  void render(SDL_Renderer* renderer);
  void move_by(int delta_x, int delta_y);
  void move_to(Position destination);
  void follow_path(Position* path, int n_nodes, bool* done);
};
