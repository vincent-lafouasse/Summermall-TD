#pragma once

#include <queue>
#include <unordered_set>
#include "geometry.h"
#include "map.h"

struct Node {
  Position position;
  Node* previous;
};

typedef std::queue<Node> Queue;

void BFS(void);
