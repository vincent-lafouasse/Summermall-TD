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

struct Map {
  const Rectangle shape;
  const Rectangle src_tileshape;
  int_vector_3D layers;
};

SDL_Rect make_tile_from_id(const int tile_id,
                           const Rectangle src_tileshape,
                           SDL_Texture* tilesheet);
void render_map(const Map map,
                SDL_Texture* tilesheet,
                const Rectangle dst_tileshape,
                SDL_Renderer* renderer);
const Map parse_map_from_tmx(const char* tmx_path);
Rectangle get_texture_shape(SDL_Texture* texture);
