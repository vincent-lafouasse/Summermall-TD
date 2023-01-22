#pragma once
#include <map>
#include <vector>
#include "geometry.h"

struct Graph {
  std::map<Position, std::vector<Position>> adjacency_list;

  void add_vertex(Position position) {
    if (adjacency_list.find(position) != adjacency_list.end()) {
      std::vector<Position> empty;
      adjacency_list[position] = empty;
    }
  }

  void add_vertices(std::vector<Position>* positions) {
    for (size_t i = 0; i < positions->size(); i++) {
      add_vertex(positions->at(i));
    }
  }

  void add_edge(Position key, Position value) {
    if (value != key) {
      adjacency_list[key].push_back(value);
    }
  }

  void add_edges(Position key, std::vector<Position>* values) {
    for (size_t i = 0; i < values->size(); i++) {
      add_edge(key, values->at(i));
    }
  }
};
