#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "geometry.h"
#include "path_finder.h"

class Monster {
 public:
  Position m_position;
  float m_orientation;
  bool m_reached_end;

 private:
  SDL_Texture* m_texture;
  Dimension m_shape;
  int m_step_id;
  int m_edge_id;

 public:
  Monster(Position position,
          float orientation,
          const Dimension dst_shape,
          SDL_Texture* texture);
  void render(SDL_Renderer* renderer);
  void follow_path(std::vector<Position> path);
  void follow_line(std::vector<Position> line);
  void step_toward_simple(Position destination);
  void move_by(int delta_x, int delta_y);
};
