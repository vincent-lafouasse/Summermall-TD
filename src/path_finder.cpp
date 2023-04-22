#include "path_finder.h"
#include <stdio.h>

void DistanceField::init(const Map* map) {
  for (int row_idx = 0; row_idx < map->shape_tl.h; row_idx++) {
    std::vector<alt_distance_t> row;
    for (int col_idx = 0; col_idx < map->shape_tl.w; col_idx++) {
      row.push_back(UNKNOWN_DISTANCE);
    }
    elements.push_back(row);
  }
}

void DistanceField::print(void) const {
  for (size_t row_idx = 0; row_idx < elements.size(); row_idx++) {
    std::vector<alt_distance_t> row = elements[row_idx];
    for (size_t col_idx = 0; col_idx < row.size(); col_idx++) {
      printf("%i ", row[col_idx]);
    }
    printf("\n");
  }
}
