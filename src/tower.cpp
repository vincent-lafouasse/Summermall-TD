#include "tower.h"

bool can_put_tower_here(Position position, std::vector<Tower>* towers) {
  return true;
}

Tower::Tower(Position position, Dimension shape, SDL_Texture* texture) {
  m_position = position;
  m_shape = shape;
  m_texture = texture;
}

void Tower::render(SDL_Renderer* renderer) {
  SDL_Rect dst_rect = {m_position.x, m_position.y, m_shape.w, m_shape.h};
  SDL_RenderCopy(renderer, m_texture, NULL, &dst_rect);
}
