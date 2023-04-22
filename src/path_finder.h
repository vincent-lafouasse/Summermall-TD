#pragma once

#include <vector>
#include "map.h"
#include "tower.h"

#define UNKNOWN_DISTANCE -1
#define UNTRAVERSABLE -2

typedef int distance_t;

struct DistanceField {
  std::vector<std::vector<distance_t>> elements;

  bool try_computing_BFS(const Map* map,
                         std::vector<Tower>* towers,
                         Position entrance,
                         Position exit);
  void init(const Map* map);
  void print(void) const;
  void set_at(Position position, distance_t distance);
  distance_t at(Position position);
};

std::vector<Position> neighboring_tiles(Position tile,
                                        const Map* map,
                                        std::vector<Tower>* towers);
