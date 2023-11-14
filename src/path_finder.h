#pragma once

#include <vector>
#include "map.h"
#include "tower.h"

#define UNKNOWN_DISTANCE -1
#define UNTRAVERSABLE -2

typedef int distance_t;

struct DistanceField {
  const Map* m_map;

  DistanceField(const Map* map) : m_map(map) { reset(); }

  std::vector<std::vector<distance_t>> elements;

  bool try_computing_BFS(std::vector<Tower>* towers,
                         Position entrance,
                         Position exit);
  void reset();
  void reset(const Map* map);
  void print(void) const;
  void set_at(Position position, distance_t distance);
  bool min_neighbour(std::vector<Tower>* towers,
                     Position position_tl,
                     Position* next_position_tl) const;
  distance_t at(Position position) const;
};
