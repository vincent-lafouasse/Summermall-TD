#include "tower.h"

bool can_put_tower_here(Position position,
                        std::vector<Tower>* towers,
                        Dimension tower_shape) {
  for (size_t i = 0; i < towers->size(); ++i) {
    Position other_position = towers->at(i).m_position;
    if ((other_position.x >= position.x - (tower_shape.w / 2) &&
         other_position.x <= position.x + (tower_shape.w / 2)) &&
        (other_position.y >= position.y - (tower_shape.w / 2) &&
         other_position.y <= position.y + (tower_shape.w / 2))) {
      return false;
    }
  }
  return true;
}

void delete_tower_at(Position position, std::vector<Tower>* towers) {
  for (size_t i = 0; i < towers->size(); ++i) {
    if (towers->at(i).m_position == position) {
      towers->erase(towers->begin() + i);
      break;
    }
  }
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
