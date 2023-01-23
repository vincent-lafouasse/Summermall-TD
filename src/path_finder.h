#pragma once
#include <stdint.h>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include "SDL2/SDL.h"
#include "geometry.h"

// Special thanks go to Amit Patel
// [Amit’s A* Pages From Red Blob Games]
// (http://theory.stanford.edu/~amitp/GameProgramming/)

typedef uint64_t distance_t;
#define INFINITE_DISTANCE UINT64_MAX
// 1.8e19 (2^64 - 1)

///
// An unweighted undirected graph of Positions
//
// @field adjacency_map A classical adjacency list.
// @field edges_repr    A vector of lines (vectors of Positions) representing
//                      the edges. Used to render the graph.
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

// Create a map with each graph node as a key, with a value representing an
// infinite distance. Infinite distance means that the node is not visited yet.
std::map<Position, distance_t> setup_distance_map(WaypointGraph* graph);

// Return the path from entrance to exit. Return an empty vector if the exit is
// unattainable.
std::vector<Position> reconstruct_path(std::map<Position, Position>* came_from,
                                       Position entrance,
                                       Position exit);

// From a vector of waypoints, return a vector of contiguous lines between the
// waypoints.
std::vector<std::vector<Position>> get_path_repr(std::vector<Position>* path);

// Distance used in graph traversal.
// Squared euclidian distance.
distance_t distance(Position from, Position to);
