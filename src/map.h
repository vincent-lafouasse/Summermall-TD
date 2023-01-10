#pragma once

#include <stdio.h>
#include <string.h>
#include <vector>
#include "third_party_lib/tinyxml2.h"

#include "geometry.h"

typedef std::vector<int> int_vector_1D;
typedef std::vector<std::vector<int>> int_vector_2D;
typedef std::vector<std::vector<std::vector<int>>> int_vector_3D;

// A representation of a map made of tiles.
//
// @field shape         The width and height of the map in tile units.
// @field src_tileshape The width and height of the tiles in the tilesheet.
// @field layers        A vector containing 2D vector representations of each
//                      layer of the map.
struct Map {
  const Dimension shape;
  const Dimension src_tileshape;
  int_vector_3D layers;
};

//
const Map parse_map_from_tmx(const char* tmx_path);
