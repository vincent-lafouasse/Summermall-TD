#pragma once
#include <map>
#include <set>
#include <vector>
#include "SDL2/SDL.h"
#include "geometry.h"

struct WaypointGraph {
  std::map<Position, std::set<Position>> adjacency_map;
  std::vector<std::vector<Position>> edges_repr;

  void add_vertex(Position position);
  void add_vertices(std::vector<Position>* positions);
  void add_edge(Position key, Position value);
  void add_edges(Position key, std::vector<Position>* values);
  void render(SDL_Renderer* renderer);
};
