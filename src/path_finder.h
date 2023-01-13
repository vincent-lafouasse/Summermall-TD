#pragma once
#include <map>
#include <vector>
#include "geometry.h"
#include "math.h"

std::vector<Position> get_Bresenham_line_between(Position start,
                                                 Position end,
                                                 float* return_orientation);
