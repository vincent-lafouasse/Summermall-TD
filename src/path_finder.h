#pragma once

#include <map>
#include <queue>
#include "geometry.h"
#include "map.h"

void BFS(Position entrance);
void add_neighbors_to_queue(Position node,
                            std::queue<Position> queue,
                            std::map<Position, Position> reached);
