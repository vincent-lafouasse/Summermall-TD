#include "path_finder.h"

void BFS(Position entrance) {
  std::queue<Position> queue;
  std::map<Position, Position> reached;

  queue.push(entrance);

  while (!queue.empty()) {
    Position root = queue.front();
    add_neighbors_to_queue(root, queue, reached);
    queue.pop();
  }
}

void add_neighbors_to_queue(Position node,
                            std::queue<Position> queue,
                            std::map<Position, Position> reached) {}
