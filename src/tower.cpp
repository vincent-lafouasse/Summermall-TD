#include "tower.h"
#include <stdlib.h>

bool can_put_tower_here(Position position_tl, std::vector<Tower>* towers) {
  for (size_t i = 0; i < towers->size(); ++i) {
    Tower tower = towers->at(i);
    if ((abs(position_tl.x - tower.m_position_tl.x) <= tower.m_size_tl / 2) &&
        (abs(position_tl.y - tower.m_position_tl.y) <= tower.m_size_tl / 2)) {
      return false;
    }
  }
  return true;
}

void delete_tower_at(Position position_tl, std::vector<Tower>* towers) {
  for (size_t i = 0; i < towers->size(); ++i) {
    if (towers->at(i).m_position_tl == position_tl) {
      towers->erase(towers->begin() + i);
      break;
    }
  }
}

Tower::Tower(Position position_tl, int size_tl, SDL_Texture* texture) {
  m_position_tl = position_tl;
  m_size_tl = size_tl;
  m_texture = texture;
}

void Tower::render(Dimension tileshape, SDL_Renderer* renderer) {
  SDL_Rect dst_rect = {m_position_tl.x * tileshape.w,
                       m_position_tl.y * tileshape.h, m_size_tl * tileshape.w,
                       m_size_tl * tileshape.h};
  SDL_RenderCopy(renderer, m_texture, NULL, &dst_rect);
}

Position Tower::get_position(Dimension tileshape) {
  return {
      m_position_tl.x * tileshape.w,
      m_position_tl.y * tileshape.h,
  };
}

bool Tower::operator<(const Tower& other) const {
  return m_position_tl < other.m_position_tl;
}
