#include "geometry.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>

#define PI 3.141592653589793238

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

Position tile_center(Position tile, Dimension tileshape) {
  return {tile.x + tileshape.w / 2, tile.y + tileshape.h / 2};
}

std::vector<Position> flatten_2D_position_vector(
    std::vector<std::vector<Position>>* vector) {
  std::vector<Position> flattened;
  for (size_t i = 0; i < vector->size(); i++) {
    std::vector<Position> sub_vector = vector->at(i);
    for (size_t j = 0; j < sub_vector.size(); j++) {
      flattened.push_back(sub_vector[j]);
    }
  }
  return flattened;
}

std::vector<Position> append_position_vector(std::vector<Position>* vec1,
                                             std::vector<Position>* vec2) {
  std::vector<Position> output = *vec1;
  for (size_t i = 0; i < vec2->size(); i++) {
    output.push_back(vec2->at(i));
  }
  return output;
}

float line_angle(std::vector<Position>* line) {
  int delta_x = (line->back().x - line->front().x);
  int delta_y = (line->back().y - line->front().y);
  if (delta_x == 0) {
    return copysign(90.0, delta_y);
  }
  float angle = atan((float)delta_y / (float)delta_x) * 180. / PI;
  return angle += (delta_x < 0) * 180.0;
}

bool line_is_vertical(Position start, Position end) {
  return end.x - start.x == 0;
}

bool is_between(int candidate, int lower_bound, int upper_bound) {
  return (lower_bound <= candidate && candidate <= upper_bound);
}

Position pos_1D_to_2D(int pos_1D, int width) {
  return {pos_1D % width, pos_1D / width};
}

int pos_2D_to_1D(Position position, int width) {
  return position.y * width + position.x;
}

Position pixel_pos_from_grid(Position position_tl, Dimension tileshape) {
  return {position_tl.x * tileshape.w, position_tl.y * tileshape.h};
}
Dimension pixel_shape_from_grid(Dimension shape_tl, Dimension tileshape) {
  return {
      shape_tl.w * tileshape.w,
      shape_tl.h * tileshape.h,
  };
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
Dimension Dimension::operator*(const int& scalar) const {
  return Dimension(scalar * w, scalar * h);
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
