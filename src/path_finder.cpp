#include "path_finder.h"

void Graph::add_vertex(Position position) {
  if (adjacency_map.find(position) != adjacency_map.end()) {
    std::set<Position> empty;
    adjacency_map[position] = empty;
  }
}

void Graph::add_vertices(std::vector<Position>* positions) {
  for (size_t i = 0; i < positions->size(); i++) {
    add_vertex(positions->at(i));
  }
}

void Graph::add_edge(Position key, Position value) {
  if (value != key) {
    adjacency_map[key].insert(value);
  }
}

void Graph::add_edges(Position key, std::vector<Position>* values) {
  for (size_t i = 0; i < values->size(); i++) {
    add_edge(key, values->at(i));
  }
}
