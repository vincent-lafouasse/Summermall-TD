#include "geometry.h"
#include <algorithm>

Position pos_1D_to_2D(int pos_1D, int width) {
  return {pos_1D % width, pos_1D / width};
}

int pos_2D_to_1D(Position position, int width) {
  return position.y * width + position.x;
}

Position pixel_pos_from_grid(Position grid_pos, Dimension tileshape) {
  return {grid_pos.x * tileshape.w, grid_pos.y * tileshape.h};
}

float distance(Position a, Position b) {
  int delta_x = a.x - b.x;
  int delta_y = a.y - b.y;
  return sqrt(delta_x * delta_x + delta_y * delta_y);
}

static std::vector<Position> Bresenham_small_delta_x(Position start,
                                                     Position end);
static std::vector<Position> Bresenham_small_delta_y(Position start,
                                                     Position end);

std::vector<Position> get_Bresenham_line_between(Position start, Position end) {
  int delta_x = end.x - start.x;
  int delta_y = end.y - start.y;
  std::vector<Position> line;

  if (abs(delta_y) < abs(delta_x)) {
    if (delta_x >= 0) {
      line = Bresenham_small_delta_y(start, end);
    } else {
      line = Bresenham_small_delta_y(end, start);
    }
  } else {
    if (delta_y >= 0) {
      line = Bresenham_small_delta_x(start, end);
    } else {
      line = Bresenham_small_delta_x(end, start);
    }
  }
  if (line[0] == end) {
    std::reverse(line.begin(), line.end());
  }
  return line;
}

static std::vector<Position> Bresenham_small_delta_y(Position start,
                                                     Position end) {
  int delta_x = end.x - start.x;
  int delta_y = end.y - start.y;
  int y_increment = 1;
  if (delta_y < 0) {
    y_increment = -1;
    delta_y = -delta_y;
  }
  int difference = 2 * delta_y - delta_x;
  int y = start.y;
  std::vector<Position> line;

  for (int x = start.x; x <= end.x; x++) {
    line.push_back({x, y});
    if (difference > 0) {
      y += y_increment;
      difference -= 2 * delta_x;
    }
    difference += 2 * delta_y;
  }
  return line;
}

static std::vector<Position> Bresenham_small_delta_x(Position start,
                                                     Position end) {
  int delta_y = end.y - start.y;
  int delta_x = end.x - start.x;
  int x_increment = 1;
  if (delta_x < 0) {
    x_increment = -1;
    delta_x = -delta_x;
  }
  int difference = 2 * delta_x - delta_y;
  int x = start.x;
  std::vector<Position> line;

  for (int y = start.y; y <= end.y; y++) {
    line.push_back({x, y});
    if (difference > 0) {
      x += x_increment;
      difference -= 2 * delta_y;
    }
    difference += 2 * delta_x;
  }
  return line;
}
