#include "enemy.h"
#include <math.h>

Monster::Monster(Position position,
                 const Dimension dst_shape,
                 SDL_Texture* texture) {
  m_position = position;
  m_shape = dst_shape;
  m_texture = texture;

  m_orientation = 0;
  m_reached_end = false;
  m_step_id = 0;
  m_edge_id = 0;
}

void Monster::follow_path(std::vector<Position>* path) {
  static std::vector<Position> line =
      get_Bresenham_line_between(path->at(m_edge_id), path->at(m_edge_id + 1));
  static float angle = line_angle(&line);
  if (m_orientation != angle) {
    m_orientation = angle;
  }
  if (m_position == line.back()) {
    m_edge_id++;
    m_step_id = 0;
    line = get_Bresenham_line_between(path->at(m_edge_id),
                                      path->at(m_edge_id + 1));
    angle = line_angle(&line);
  }
  follow_line(&line);

  m_reached_end = m_position == path->back();
}

void Monster::follow_line(std::vector<Position>* line) {
  step_toward_simple(line->at(m_step_id));
  if (m_position == line->at(m_step_id))
    m_step_id++;
}

int sign_of(int number) {
  return (number != 0) * (number > 0 ? 1 : -1);
}

void Monster::step_toward_simple(Position destination) {
  move_by(sign_of(destination.x - m_position.x),
          sign_of(destination.y - m_position.y));
}

void Monster::move_by(int delta_x, int delta_y) {
  m_position.x += delta_x;
  m_position.y += delta_y;
}

void Monster::render(SDL_Renderer* renderer) {
  SDL_Rect dst_tile_loc = {m_position.x - (m_shape.w / 2),
                           m_position.y - (m_shape.h / 2), m_shape.w,
                           m_shape.h};
  SDL_RenderCopyEx(renderer, m_texture, NULL, &dst_tile_loc, m_orientation,
                   NULL, SDL_FLIP_NONE);
}
