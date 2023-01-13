#include "enemy.h"
#include <math.h>
#include <vector>

Monster::Monster(Position position,
                 float orientation,
                 const Dimension dst_shape,
                 SDL_Texture* texture) {
  m_orientation = orientation;
  m_shape = dst_shape;
  m_texture = texture;
  m_position = position;
  m_reached_end = false;
  m_step_id = 0;
  m_edge_id = 0;
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

void Monster::step_toward_simple(Position destination) {
  int delta_x = destination.x - m_position.x;
  int delta_y = destination.y - m_position.y;
  move_by(sign_of(delta_x), sign_of(delta_y));
}

void Monster::follow_path(std::vector<Position> path) {
  static auto line = get_Bresenham_line_between(
      path[m_edge_id], path[m_edge_id + 1], &m_orientation);
  follow_line(line);

  m_reached_end = m_position == path[path.size() - 1];
}

void Monster::follow_line(std::vector<Position> line) {
  step_toward_simple(line[m_step_id]);
  if (m_position == line[m_step_id])
    m_step_id++;
}
