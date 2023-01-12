#include "enemy.h"

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

void Monster::move_to(Position destination) {
  int x_movement =
      (m_position.x != destination.x) * (m_position.x > destination.x ? -1 : 1);
  int y_movement =
      (m_position.y != destination.y) * (m_position.y > destination.y ? -1 : 1);
  move_by(x_movement, y_movement);
}

void Monster::follow_path(Position* path, int n_nodes) {}
