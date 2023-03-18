#include "tower.h"

static int max_int(int a, int b);
static int min_int(int a, int b);

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

bool line_passes_through_tower(Position start,
                               Position end,
                               std::vector<Tower>* towers,
                               Dimension tower_shape) {
  int line_big_x = max_int(start.x, end.x);
  int line_small_x = min_int(start.x, end.x);
  int line_big_y = max_int(start.y, end.y);
  int line_small_y = min_int(start.y, end.y);
  int tower_small_x;
  int tower_small_y;
  int tower_big_x;
  int tower_big_y;
  if (line_is_vertical(start, end)) {
    int line_x = line_big_x;
    for (size_t i = 0; i < towers->size(); i++) {
      get_tower_borders(towers->at(i), tower_shape, &tower_small_x,
                        &tower_big_x, &tower_small_y, &tower_big_y);
      bool tower_is_vertically_in_bound =
          is_between(line_x, tower_small_x, tower_big_x);
      bool tower_is_horizontally_in_bound =
          (tower_small_y <= line_big_y) && (tower_big_y >= line_small_y);
      if (tower_is_horizontally_in_bound && tower_is_vertically_in_bound) {
        return true;
      }
    }
  } else {
    // line is y = ax + b with:
    double a = (double)(start.y - end.y) / (double)(start.x - end.x);
    double b = (double)start.y - a * start.x;
    for (size_t i = 0; i < towers->size(); i++) {
      get_tower_borders(towers->at(i), tower_shape, &tower_small_x,
                        &tower_big_x, &tower_small_y, &tower_big_y);
      if (tower_small_x > line_big_x || tower_small_y > line_big_y ||
          tower_big_x < line_small_x || tower_big_y < line_small_y) {
        continue;
      }

      // Check for intersection of the line with the tower's vertical borders
      int y_possible_left_intersection = (int)(a * tower_small_x + b);
      int y_possible_right_intersection = (int)(a * tower_big_x + b);
      bool line_interects_left_border =
          is_between(y_possible_left_intersection, tower_small_y, tower_big_y);
      bool line_interects_right_border =
          is_between(y_possible_right_intersection, tower_small_y, tower_big_y);
      if (line_interects_left_border || line_interects_right_border) {
        return true;
      }

      // Check for intersection of the line with the tower's horizontal borders
      int x_possible_top_intersection = (int)((1 / a) * (tower_small_y - b));
      int x_possible_bottom_intersection = (int)((1 / a) * (tower_big_y - b));
      bool line_interects_top_border =
          is_between(x_possible_top_intersection, tower_small_x, tower_big_x);
      bool line_interects_bottom_border = is_between(
          x_possible_bottom_intersection, tower_small_x, tower_big_x);
      if (line_interects_top_border || line_interects_bottom_border) {
        return true;
      }
    }
  }
  return false;
}

bool Tower::operator<(const Tower& other) const {
  return m_position < other.m_position;
}

void get_tower_borders(Tower tower,
                       Dimension tower_shape,
                       int* return_small_x,
                       int* return_big_x,
                       int* return_small_y,
                       int* return_big_y) {
  *return_small_x = tower.m_position.x;
  *return_big_x = tower.m_position.x + tower_shape.w;
  *return_small_y = tower.m_position.y;
  *return_big_y = tower.m_position.y + tower_shape.h;
}

int max_int(int a, int b) {
  return a > b ? a : b;
}
int min_int(int a, int b) {
  return a < b ? a : b;
}
