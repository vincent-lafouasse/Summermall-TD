#include "path_finder.h"
#include <SDL2/SDL.h>
#include "render.h"

std::vector<Position> Dijkstra_shortest_path(WaypointGraph* graph,
                                             Position entrance,
                                             Position exit) {
  using namespace std;
  // Create a distance map with infinite distance for all graph nodes
  map<Position, distance_t> distance_from_entrance = setup_distance_map(graph);
  distance_from_entrance[entrance] = 0;
  map<Position, Position> came_from;
  came_from[entrance] = entrance;
  PriorityQueue queue;
  queue.put(entrance, 0);

  while (!queue.is_empty()) {
    Position current = queue.pop();
    set<Position> neighbours = graph->adjacency_map[current];

    for (Position candidate : neighbours) {
      // Calculate distance from entrance to candidate, passing through current
      distance_t new_distance =
          distance_from_entrance[current] + distance(current, candidate);

      if (new_distance < distance_from_entrance[candidate]) {
        // If going to candidate through current is a shortcut:
        distance_from_entrance[candidate] = new_distance;
        came_from[candidate] = current;
        queue.put(candidate, new_distance);
      }
    }
  }

  return reconstruct_path(&came_from, entrance, exit);
}

std::vector<Position> reconstruct_path(std::map<Position, Position>* came_from,
                                       Position entrance,
                                       Position exit) {
  bool exit_is_reached = came_from->find(exit) == came_from->end();
  if (!exit_is_reached) {
    std::vector<Position> empty;
    return empty;
  }
  std::vector<Position> path;
  Position current = exit;

  while (current != entrance) {
    path.push_back(current);
    current = (*came_from)[current];
  }

  path.push_back(entrance);
  std::reverse(path.begin(), path.end());
  return path;
}

distance_t distance(Position from, Position to) {
  distance_t delta_x = from.x - to.x;
  distance_t delta_y = from.y - to.y;
  return (delta_x * delta_x) + (delta_y * delta_y);
}

std::map<Position, distance_t> setup_distance_map(WaypointGraph* graph) {
  auto adjacency_map = graph->adjacency_map;
  std::map<Position, distance_t> distance_map;

  for (auto it = adjacency_map.begin(); it != adjacency_map.end(); ++it) {
    distance_map[it->first] = INFINITE_DISTANCE;
  }

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

std::vector<std::vector<Position>> get_path_repr(std::vector<Position>* path) {
  std::vector<std::vector<Position>> path_repr;
  for (size_t i = 0; i < path->size() - 1; i++) {
    path_repr.push_back(
        get_Bresenham_line_between(path->at(i), path->at(i + 1)));
  }
  return path_repr;
}
