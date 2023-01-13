#include "path_finder.h"

std::vector<Position> get_Bresenham_line_between(Position start,
                                                 Position end,
                                                 float* return_orientation) {
  int delta_x = end.x - start.x;
  int delta_y = end.y - start.y;
  int difference = 2 * delta_y - delta_x;
  int y = start.y;

  std::vector<Position> line;

  for (int x = start.x; x <= end.x; x++) {
    line.push_back({x, y});
    if (difference > 0) {
      y++;
      difference -= 2 * delta_x;
    }
    difference += 2 * delta_y;
  }
  *return_orientation = atan((float)delta_y / (float)delta_x);
  return line;
}
