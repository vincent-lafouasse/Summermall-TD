#include "map.h"

SDL_Rect get_tile_from_id(const int id,
                          const Rectangle src_tileshape,
                          const int tilesheet_width_pixel) {
  const int actual_id = id - 1;
  const int tilesheet_width = tilesheet_width_pixel / src_tileshape.w;
  const int X = (actual_id % tilesheet_width) * src_tileshape.w;
  const int Y = (actual_id / tilesheet_width) * src_tileshape.h;
  const SDL_Rect tile = {X, Y, src_tileshape.w, src_tileshape.h};
  return tile;
}

void render_map(const Map map,
                Tilesheet tilesheet,
                Rectangle dst_tileshape,
                SDL_Renderer* renderer) {
  for (size_t layer_index = 0; layer_index < map.layers.size(); layer_index++) {
    int_vector_2D layer = map.layers[layer_index];

    for (int y_pos = 0; y_pos < (int)layer.size(); y_pos++) {
      int_vector_1D line = layer[y_pos];

      for (int x_pos = 0; x_pos < (int)line.size(); x_pos++) {
        SDL_Rect src_tile = get_tile_from_id(line[x_pos], map.src_tileshape,
                                             tilesheet.pixel_width);
        SDL_Rect dst_tile = {x_pos * dst_tileshape.w, y_pos * dst_tileshape.h,
                             dst_tileshape.w, dst_tileshape.h};
        SDL_RenderCopy(renderer, tilesheet.texture, &src_tile, &dst_tile);
      }
    }
  }
}

const Map make_map_from_tmx(const char* tmx_path) {
  using namespace tinyxml2;
  XMLDocument map_xml;

  // Load tmx
  printf("loading tmx: %s\n",
         map_xml.LoadFile(tmx_path) == XML_SUCCESS ? "done" : "fail");

  // Extract metadata
  XMLElement* root = map_xml.FirstChildElement("map");
  const Rectangle map_shape = {atoi(root->Attribute("width")),
                               atoi(root->Attribute("height"))};
  const Rectangle map_src_tileshape = {atoi(root->Attribute("tilewidth")),
                                       atoi(root->Attribute("tileheight"))};

  // Extract map layers into 3D vector from csv
  XMLElement* layer = root->FirstChildElement("layer");
  char* layer_csv;
  int_vector_3D layers;
  layers.clear();
  while (layer) {
    layer_csv = (char*)layer->FirstChildElement("data")->GetText();

    layers.push_back(vector_2D_from_string_csv(layer_csv));

    layer = layer->NextSiblingElement();
  }
  Map map = {map_shape, map_src_tileshape, layers};
  return map;
}

int_vector_2D vector_2D_from_string_csv(char* csv_string) {
  int_vector_2D output;
  const char* line_delim = "\n";
  char* line = NULL;
  char* mem;
  output.clear();

  line = strtok_r(csv_string, line_delim, &mem);
  while (line != NULL) {
    int_vector_1D res = vector_1D_from_string_line(line);
    output.push_back(res);
    line = strtok_r(NULL, line_delim, &mem);
  }

  return output;
}

int_vector_1D vector_1D_from_string_line(char* line_string) {
  int_vector_1D output;
  const char* cell_delim = ",";
  char* cell = NULL;
  char* mem;
  output.clear();

  cell = strtok_r(line_string, cell_delim, &mem);
  while (cell != NULL) {
    output.push_back(atoi(cell));
    cell = strtok_r(NULL, cell_delim, &mem);
  }
  return output;
}

void print_1D_int_vector(int_vector_1D const input) {
  for (unsigned long i = 0; i < input.size(); i++) {
    printf("%d ", input[i]);
  }
  printf("\n");
}

void print_2D_int_vector(int_vector_2D const input) {
  for (unsigned long i = 0; i < input.size(); i++) {
    print_1D_int_vector(input[i]);
  }
}
