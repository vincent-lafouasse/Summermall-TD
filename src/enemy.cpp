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

std::vector<Position> get_Bresenham_line_between(Position start, Position end) {
  int delta_x = end.x - start.x;
  int delta_y = end.y - start.y;
  int difference = 2 * delta_y - delta_x;
  int y = start.y;
  std::vector<Position> path;

  for (int x = start.x; x <= end.x; x++) {
    path.push_back({x, y});
    if (difference > 0) {
      y++;
      difference -= 2 * delta_x;
    }
    difference += delta_y;
  }
  return path;
}

void Monster::move_linearly_to(Position destination) {
  // TODO implement Bresenham line algorithm
  int x_movement, y_movement;
  int delta_x = destination.x - m_position.x;
  int delta_y = destination.y - m_position.y;

  x_movement = sign_of(delta_x);
  y_movement = sign_of(delta_y);

  move_by(x_movement, y_movement);
}

void Monster::follow_path(Position* path, int n_nodes, bool* done) {
  static int current_edge = 0;
  if (m_position == path[current_edge])
    current_edge++;
  move_linearly_to(path[current_edge]);

  *done = (m_position == path[n_nodes - 1]);
}
