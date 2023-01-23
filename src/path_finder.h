#pragma once
#include <stdint.h>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include "SDL2/SDL.h"
#include "geometry.h"

// Special thanks go to Amit Patel redblobgames@gmail.com
// [Amit’s A* Pages From Red Blob
// Games](http://theory.stanford.edu/~amitp/GameProgramming/)

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

///
// A wrapper for std::priority_queue
//
// Made so that the method names follow the semantics and to hide the internals.
//
// @field elements The queue itself. It is made of std::pair<distance_t,
//                 Position> elements ordered so that the element at the top is
//                 the one with the smallest distance.
//
// @method empty Return true if the queue is empty, false otherwise.
// @method put   Taking a distance and a position, construct the std::pair,
//               insert it in the queue and sort the queue.
// @method get   Pop the top element and return its Position.
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
std::map<Position, distance_t> setup_distance_map(WaypointGraph* graph);
distance_t distance(Position from, Position to);
std::vector<Position> reconstruct_path(std::map<Position, Position>* came_from,
                                       Position entrance,
                                       Position exit);
std::vector<std::vector<Position>> get_path_repr(std::vector<Position>* path);
