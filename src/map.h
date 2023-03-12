#pragma once

#include <set>
#include <vector>
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
  std::set<Position> non_buildable_tiles;
};

// Kenney tilesheet tile ids
// used to parse buildable vs non-buildable tiles
enum GroundTile : int {
  GRASS = 96,
  WALL = 392,
  SAND = 565,
  DIRT = 1,
};
//
const Map parse_map_from_tmx(const char* tmx_path);

std::set<Position> get_non_buildable_tiles_set(int_vector_2D* ground_layer);
