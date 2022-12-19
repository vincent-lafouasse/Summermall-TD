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
  Rectangle shape;
  Rectangle tileshape;
  int_vector_3D layers;
};

struct Tilesheet {
  SDL_Texture* texture;
  int width;
};

SDL_Rect get_tile_from_id(int id, int tilesheet_width, Rectangle tileshape);
void render_map(Map map,
                Tilesheet tilesheet,
                Rectangle shape_rendered_tile,
                SDL_Renderer* renderer);
Map make_map_from_tmx(const char* tmx_path);
int_vector_2D vector_2D_from_string_csv(char* csv_string);
int_vector_1D vector_1D_from_string_line(char* line_string);
void print_1D_int_vector(const int_vector_1D input);
void print_2D_int_vector(const int_vector_2D input);
