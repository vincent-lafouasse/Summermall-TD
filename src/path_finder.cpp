#include "path_finder.h"
#include <SDL2/SDL.h>

void WaypointGraph::add_vertex(Position position) {
  if (adjacency_map.find(position) != adjacency_map.end()) {
    std::set<Position> empty;
    adjacency_map[position] = empty;
  }
}

void WaypointGraph::add_vertices(std::vector<Position>* positions) {
  for (size_t i = 0; i < positions->size(); i++) {
    add_vertex(positions->at(i));
  }
}

void WaypointGraph::add_edge(Position key, Position value) {
  if (value != key) {
    adjacency_map[key].insert(value);
    adjacency_map[value].insert(key);
    edges_repr.push_back(get_Bresenham_line_between(key, value));
  }
}

void WaypointGraph::add_edges(Position key, std::vector<Position>* values) {
  for (size_t i = 0; i < values->size(); i++) {
    add_edge(key, values->at(i));
  }
}

void WaypointGraph::render(void) {}
