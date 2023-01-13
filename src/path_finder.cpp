#include "path_finder.h"
#include <algorithm>

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

std::vector<Position> get_Bresenham_line_between(Position start,
                                                 Position end,
                                                 float* return_orientation) {
  int delta_x = end.x - start.x;
  int delta_y = end.y - start.y;
  std::vector<Position> line;

  if (abs(delta_y) < abs(delta_x)) {
    if (delta_x >= 0) {
      line = Bresenham_small_delta_y(start, end);
    } else {
      line = Bresenham_small_delta_y(end, start);
      std::reverse(line.begin(), line.end());
    }
  } else {
    if (delta_y >= 0) {
      line = Bresenham_small_delta_x(start, end);
    } else {
      line = Bresenham_small_delta_x(end, start);
      std::reverse(line.begin(), line.end());
    }
  }

  *return_orientation = atan((float)delta_y / (float)delta_x);
  return line;
}
