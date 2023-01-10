#pragma once

#include <queue>
#include <vector>
#include "geometry.h"
#include "map.h"

typedef std::queue<Node> Queue;

struct Node {
  Position position;
  Node* previous;
  Node* next;
};

void BFS(Position entrance_pos, Position exit_pos, Map* map);
void add_neighbours_to_queue(Node node,
                             Map* map,
                             std::vector<Node> reached,
                             Queue queue);
