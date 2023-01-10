#pragma once

#include <queue>
#include <vector>
#include "geometry.h"
#include "map.h"

struct Node {
  Position position;
  Node* previous;
  Node* next;
};

typedef std::queue<Node> Queue;

void BFS(Position entrance_pos, Position exit_pos, Map* map);
void add_neighbours_to_queue(Node node, Map* map, Queue queue);
