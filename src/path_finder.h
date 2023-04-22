#pragma once

#include <vector>
#include "map.h"
#include "tower.h"

#define UNKNOWN_DISTANCE -1
#define UNTRAVERSABLE -2

typedef int alt_distance_t;

struct DistanceField {
  std::vector<std::vector<alt_distance_t>> elements;

  void init(const Map* map);
  void print(void) const;
};
