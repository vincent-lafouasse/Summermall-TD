#include "geometry.h"

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
