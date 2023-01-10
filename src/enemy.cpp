#include "enemy.h"

Monster::Monster(Position position,
                 float orientation,
                 const Rectangle dst_shape,
                 SDL_Texture* texture) {
  m_position = position;
  m_orientation = orientation;
  m_shape = dst_shape;
  m_texture = texture;
}

void Monster::move_by(int delta_x, int delta_y) {
  m_position.x += delta_x;
  m_position.y += delta_y;
}

void Monster::render(SDL_Renderer* renderer) {
  SDL_Rect dst_tile_loc = {m_position.x, m_position.y, m_shape.w, m_shape.h};
  SDL_RenderCopy(renderer, m_texture, NULL, &dst_tile_loc);
}

