#pragma once

#include <map>
#include <queue>
#include "geometry.h"
#include "map.h"

void BFS(Position entrance, Map* map);
Position* get_neighbours(Position node, Map* map, int* n_neighbours);
void add_neighbours_to_queue(Position node,
                             std::queue<Position> queue,
                             std::map<Position, Position> reached,
                             Map* map);
