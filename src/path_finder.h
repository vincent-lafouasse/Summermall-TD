#pragma once
#include <stdint.h>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include "SDL2/SDL.h"
#include "geometry.h"

#define distance_t uint64_t
#define INFINITE_DISTANCE UINT64_MAX
// 1.8e19 (2^64 - 1)

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
  typedef std::pair<distance_t, Position> PQElement;
  // distance goes first so that it's what primarely compared by std::greater.
  // If distance is the same, they are then compared by position but we don't
  // care about this
  std::
      priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>>
          elements;
  // std::greater so that the smallest element is at the top

  inline bool empty() const { return elements.empty(); }

  inline void put(Position item, distance_t priority) {
    elements.emplace(priority, item);
    // `emplace` constructs, inserts and sorts
  }

  Position get() {
    // Dequeue the top element, i.e. element with smallest distance
    Position best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

std::vector<Position> Dijkstra_shortest_path(WaypointGraph* graph,
                                             Position entrance,
                                             Position exit);
std::map<Position, distance_t> setup_distance_map(WaypointGraph* graph);
distance_t cost(Position from, Position to);
std::vector<Position> reconstruct_path(std::map<Position, Position>* came_from,
                                       Position entrance,
                                       Position exit);
std::vector<std::vector<Position>> get_path_repr(std::vector<Position>* path);
