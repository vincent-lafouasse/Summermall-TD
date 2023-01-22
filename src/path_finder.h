#pragma once
#include <map>
#include <set>
#include <vector>
#include "geometry.h"

struct Graph {
  std::map<Position, std::set<Position>> adjacency_map;

  void add_vertex(Position position);
  void add_vertices(std::vector<Position>* positions);
  void add_edge(Position key, Position value);
  void add_edges(Position key, std::vector<Position>* values);
};
