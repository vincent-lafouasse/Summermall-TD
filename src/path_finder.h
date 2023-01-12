#pragma once
#include <map>
#include <vector>
#include "geometry.h"

std::map<Position, std::vector<Position>> make_graph(void);
Position* get_path(void);
void add_edge(void);
void add_vertex(void);
