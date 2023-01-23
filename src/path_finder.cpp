#include "path_finder.h"
#include <SDL2/SDL.h>
#include "render.h"

#define INFINITE_DISTANCE UINT64_MAX
// 1.8e19 (2^64 - 1)

std::vector<Position> Dijkstra_shortest_path(WaypointGraph* graph,
                                             Position entrance,
                                             Position exit) {
  using namespace std;
  vector<Position> path;
  // Create a distance map with infinite distance for all except entrance
  map<Position, distance_t> distance_map = setup_distance_map(graph, entrance);
  map<Position, Position> came_from;
  PriorityQueue queue;
  queue.put(entrance, 0);
	came_from[entrance] = entrance;

  while (!queue.empty()) {
		Position current = queue.get();
		if (current == exit) {
			break;
		}
	}

  return path;
}

distance_t cost(Position from, Position to) {
  return (from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y);
}
std::map<Position, distance_t> setup_distance_map(WaypointGraph* graph,
                                                Position entrance) {
  auto adjacency_map = graph->adjacency_map;
  std::map<Position, distance_t> distance_map;
  for (auto it = adjacency_map.begin(); it != adjacency_map.end(); ++it) {
    distance_map[it->first] = INFINITE_DISTANCE;
  }
  distance_map[entrance] = 0;
  return distance_map;
}

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

void WaypointGraph::add_edge(Position node1, Position node2) {
  if (node1 != node2) {
    adjacency_map[node1].insert(node2);
    adjacency_map[node2].insert(node1);
    edges_repr.push_back(get_Bresenham_line_between(node1, node2));
  }
}

void WaypointGraph::add_edges(Position key, std::vector<Position>* values) {
  for (size_t i = 0; i < values->size(); i++) {
    add_edge(key, values->at(i));
  }
}

void WaypointGraph::render(SDL_Renderer* renderer) {
  for (size_t i = 0; i < edges_repr.size(); i++) {
    render_vector(&(edges_repr[i]), renderer);
  }
}
