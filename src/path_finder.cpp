#include "path_finder.h"

void BFS(Position entrance_pos, Position exit_pos, Map* map) {
  Node entrance = {entrance_pos, NULL, NULL};
  Queue queue;
  std::vector<Node> reached;
  queue.push(entrance);

  while (!queue.empty()) {
    Node current_node = queue.front();
    add_neighbours_to_queue(current_node, map, reached, queue);
    queue.pop();
  }
}

void add_neighbours_to_queue(Node node,
                             Map* map,
                             std::vector<Node> reached,
                             Queue queue) {}
