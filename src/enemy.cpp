#include "enemy.h"
#include <math.h>

Monster::Monster(Position position,
                 float orientation,
                 const Dimension dst_shape,
                 SDL_Texture* texture) {
  m_orientation = orientation;
  m_shape = dst_shape;
  m_texture = texture;
  m_position = position;
}

void Monster::move_by(int delta_x, int delta_y) {
  m_position.x += delta_x;
  m_position.y += delta_y;
}

void Monster::render(SDL_Renderer* renderer) {
  Position centered_position = {
      m_position.x - (m_shape.w / 2),
      m_position.y - (m_shape.h / 2),
  };
  SDL_Rect dst_tile_loc = {centered_position.x, centered_position.y, m_shape.w,
                           m_shape.h};
  SDL_RenderCopyEx(renderer, m_texture, NULL, &dst_tile_loc, m_orientation,
                   NULL, SDL_FLIP_NONE);
}

int sign_of(float number) {
  return (number != 0) * (number > 0 ? 1 : -1);
}

int sign_of(int number) {
  return (number != 0) * (number > 0 ? 1 : -1);
}

void Monster::move_to(Position destination) {
  // TODO implement Bresenham line algorithm
  int delta_x = destination.x - m_position.x;
  int delta_y = destination.y - m_position.y;
  int x_movement = sign_of(delta_x);
  int y_movement = sign_of(delta_y);
  move_by(x_movement, y_movement);
}

void Monster::follow_path(Position* path, int n_nodes, bool* done) {
  static int current_edge = 0;
  if (m_position == path[current_edge])
    current_edge++;
  move_to(path[current_edge]);

  *done = (m_position == path[n_nodes - 1]);
}
