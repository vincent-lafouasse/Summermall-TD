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

struct Tilesheet {
  SDL_Texture* texture;
  const int pixel_width;
};

SDL_Rect get_tile_from_id(const int id,
                          const Rectangle src_tileshape,
                          const int tilesheet_width_pixel);
void render_map(const Map map,
                Tilesheet tilesheet,
                const Rectangle dst_tileshape,
                SDL_Renderer* renderer);
const Map make_map_from_tmx(const char* tmx_path);
int_vector_2D vector_2D_from_string_csv(char* csv_string);
int_vector_1D vector_1D_from_string_line(char* line_string);
void print_1D_int_vector(const int_vector_1D input);
void print_2D_int_vector(const int_vector_2D input);
