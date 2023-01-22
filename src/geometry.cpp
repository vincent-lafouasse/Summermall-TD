#include "geometry.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>

#define PI 3.141592653589793238

float line_angle(std::vector<Position>* line) {
  int delta_x = (line->back().x - line->front().x);
  int delta_y = (line->back().y - line->front().y);
  if (delta_x == 0) {
    return copysign(90.0, delta_y);
  }
  float angle = atan((float)delta_y / (float)delta_x) * 180. / PI;
  return angle += (delta_x < 0) * 180.0;
}

int squared_euclidian_distance(Position a, Position b) {
  int delta_x = a.x - b.x;
  int delta_y = a.y - b.y;
  return (delta_x * delta_x + delta_y * delta_y);
}

float euclidian_distance(Position a, Position b) {
  return sqrt(squared_euclidian_distance(a, b));
}

static std::vector<Position> Bresenham_y_driving_axis(Position start,
                                                      Position end);
static std::vector<Position> Bresenham_x_driving_axis(Position start,
                                                      Position end);

std::vector<Position> get_Bresenham_line_between(Position start, Position end) {
  int delta_x = end.x - start.x;
  int delta_y = end.y - start.y;
  std::vector<Position> line;

  if (abs(delta_y) < abs(delta_x)) {
    // small delta_y => driving axis is x
    if (delta_x >= 0) {
      line = Bresenham_x_driving_axis(start, end);
    } else {
      line = Bresenham_x_driving_axis(end, start);
    }
  } else {
    // small delta_x => driving axis is y
    // same algorighm with x and y swapped
    if (delta_y >= 0) {
      line = Bresenham_y_driving_axis(start, end);
    } else {
      line = Bresenham_y_driving_axis(end, start);
    }
  }
  if (line[0] == end) {
    std::reverse(line.begin(), line.end());
  }
  return line;
}

static std::vector<Position> Bresenham_x_driving_axis(Position start,
                                                      Position end) {
  int delta_x = end.x - start.x;
  int delta_y = end.y - start.y;
  int y_increment = 1;
  if (delta_y < 0) {
    y_increment = -1;
    delta_y = -delta_y;
  }
  int deviation = 2 * delta_y - delta_x;
  int y = start.y;
  std::vector<Position> line;

  for (int x = start.x; x <= end.x; x++) {
    line.push_back({x, y});
    if (deviation > 0) {
      y += y_increment;
      deviation -= 2 * delta_x;
    }
    deviation += 2 * delta_y;
  }
  return line;
}

static std::vector<Position> Bresenham_y_driving_axis(Position start,
                                                      Position end) {
  int delta_y = end.y - start.y;
  int delta_x = end.x - start.x;
  int x_increment = 1;
  if (delta_x < 0) {
    x_increment = -1;
    delta_x = -delta_x;
  }
  int deviation = 2 * delta_x - delta_y;
  int x = start.x;
  std::vector<Position> line;

  for (int y = start.y; y <= end.y; y++) {
    line.push_back({x, y});
    if (deviation > 0) {
      x += x_increment;
      deviation -= 2 * delta_y;
    }
    deviation += 2 * delta_x;
  }
  return line;
}

Position pos_1D_to_2D(int pos_1D, int width) {
  return {pos_1D % width, pos_1D / width};
}

int pos_2D_to_1D(Position position, int width) {
  return position.y * width + position.x;
}

Position pixel_pos_from_grid(Position grid_pos, Dimension tileshape) {
  return {grid_pos.x * tileshape.w, grid_pos.y * tileshape.h};
}

void Position::print(void) {
  printf("Position(x = %d, y = %d)\n", x, y);
}

bool Dimension::operator<(const Dimension& other) const {
  return w < other.w || (w == other.w && h < other.h);
}
bool Dimension::operator==(const Dimension& other) const {
  return (w == other.w) && (h == other.h);
}
Dimension Dimension::operator+(const Dimension& other) const {
  return Dimension(w + other.w, h + other.h);
}

bool Position::operator<(const Position& other) const {
  return x < other.x || (x == other.x && y < other.y);
}
bool Position::operator>(const Position& other) const {
  return x > other.x || (x == other.x && y > other.y);
}
bool Position::operator==(const Position& other) const {
  return (x == other.x) && (y == other.y);
}
bool Position::operator!=(const Position& other) const {
  return (x != other.x) || (y != other.y);
}
Position Position::operator+(const Position& other) const {
  return Position(x + other.x, y + other.y);
}
