#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "third_party_lib/tinyxml2.h"

typedef std::vector<int> int_vector_1D;
typedef std::vector<std::vector<int>> int_vector_2D;
typedef std::vector<std::vector<std::vector<int>>> int_vector_3D;

struct Rectangle {
  int w;
  int h;
};

// A representation of a map made of tiles.
//
// @field shape         The width and height of the map in tile units.
// @field src_tileshape The width and height of the tiles in the tilesheet.
// @field layers        A vector containing 2D vector representations of each
//                      layer of the map.
struct Map {
  const Rectangle shape;
  const Rectangle src_tileshape;
  int_vector_3D layers;
};

// Give the position and shape of the tile numbered `tiled_id` in the tilesheet.
//
// @param tile_id       The integer encoding the tile.
// @param src_tileshape The width and height of the tiles in the tilesheet,
//                      i.e. how the tilesheet is to be subdivided.
// @param tilesheet     The `SDL_Texture` containing the tilesheet.
//
// @return A `SDL_Rect` containing the position and shape of the tile.
SDL_Rect make_tile_from_id(const int tile_id,
                           const Rectangle src_tileshape,
                           SDL_Texture* tilesheet);

// Show map on screen.
//
// @param map           A representation of the map to be rendered on screen.
// @param tilesheet     The tilesheet from which to extract the tiles.
// @param dst_tileshape The onscreen width and height of the tiles.
// @param rendere       The object in charge of rendering textures.
void render_map(const Map map,
                SDL_Texture* tilesheet,
                const Rectangle dst_tileshape,
                SDL_Renderer* renderer);

//
const Map parse_map_from_tmx(const char* tmx_path);

// Return the width and height in pixels of parameter `texture`.
//
// @param texture The `SDL_Texture` to characterise.
//
// @return A Rectangle object containing the width and height of `texture`.
Rectangle get_texture_shape(SDL_Texture* texture);
