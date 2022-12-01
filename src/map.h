#include <stdio.h>
#include <string>
#include <vector>
#include "third_party_lib/tinyxml2.h"

#pragma once

typedef std::vector<int> int_vector_1D;
typedef std::vector<std::vector<int>> int_vector_2D;
typedef std::vector<std::vector<std::vector<int>>> int_vector_3D;

struct Map {
  unsigned int width;
  unsigned int height;
  unsigned int tilewidth;
  unsigned int tileheight;
  int_vector_3D layers;
};

Map make_map_from_tmx(char* tmx_path);
int_vector_2D vector_2D_from_string_csv(char* csv_string);
int_vector_1D vector_1D_from_string_line(char* line_string);
void print_1D_int_vector(int_vector_1D const input);
void print_2D_int_vector(int_vector_2D const input);
