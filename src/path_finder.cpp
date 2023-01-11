#include "path_finder.h"

void BFS(Position entrance, Map* map) {
  std::queue<Position> queue;
  std::map<Position, Position> reached;

  queue.push(entrance);

  while (!queue.empty()) {
    Position parent = queue.front();
    add_neighbours_to_queue(parent, queue, reached, map);
    queue.pop();
  }
}

void add_neighbours_to_queue(Position parent,
                             std::queue<Position> queue,
                             std::map<Position, Position> reached,
                             Map* map) {
  int n_neighbours;
  Position* neighbours = get_neighbours(parent, map, &n_neighbours);

  for (int i = 0; i < n_neighbours; i++) {
    auto iterator = reached.find(neighbours[i]);
    if (iterator != reached.end()) {
      Position child = iterator->first;
    }
  }
}

Position* get_neighbours(Position node, Map* map, int* n_neighbours) {
  Position* neighbours = {};
  *n_neighbours = sizeof(neighbours) / sizeof(Position);
  return neighbours;
}
