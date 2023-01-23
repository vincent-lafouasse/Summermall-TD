#pragma once
#include <stdint.h>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include "SDL2/SDL.h"
#include "geometry.h"

struct WaypointGraph {
  std::map<Position, std::set<Position>> adjacency_map;
  std::vector<std::vector<Position>> edges_repr;

  void add_vertex(Position position);
  void add_vertices(std::vector<Position>* positions);
  void add_edge(Position node1, Position node2);
  void add_edges(Position key, std::vector<Position>* values);
  void render(SDL_Renderer* renderer);
};

struct PriorityQueue {
  typedef std::pair<uint64_t, Position> PQElement;
  std::
      priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>>
          elements;

  inline bool empty() const { return elements.empty(); }

  inline void put(Position item, uint64_t priority) {
    elements.emplace(priority, item);
  }

  Position get() {
    Position best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

std::vector<Position> Dijkstra_shortest_path(WaypointGraph* graph,
                                             Position entrance,
                                             Position exit);
std::map<Position, uint64_t> setup_distance_map(WaypointGraph* graph,
                                                Position entrance);
uint64_t cost(Position from, Position to);
