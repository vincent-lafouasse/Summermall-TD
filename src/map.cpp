#include "map.h"

SDL_Rect get_tile_from_id(int id,
                          int tilesheet_width,
                          int tilewidth,
                          int tileheight) {
  int actual_id = id - 1;
  int X = (actual_id % tilesheet_width) * tilewidth;
  int Y = (actual_id / tilesheet_width) * tileheight;
  SDL_Rect tile = {X, Y, (int)tilewidth, (int)tileheight};
  return tile;
}

void render_map(Map map,
                SDL_Texture* tilesheet,
                int tilesheet_width,
                int render_tilewidth,
                int render_tileheight,
                SDL_Renderer* renderer) {
  for (size_t layer_index = 0; layer_index < map.layers.size(); layer_index++) {
    int_vector_2D layer = map.layers.at(layer_index);

    for (int y_pos = 0; y_pos < (int)layer.size(); y_pos++) {
      int_vector_1D line = layer.at(y_pos);

      for (int x_pos = 0; x_pos < (int)line.size(); x_pos++) {
        SDL_Rect tile = get_tile_from_id(line.at(x_pos), tilesheet_width,
                                         map.tilewidth, map.tileheight);
        SDL_Rect tile_pos = {x_pos * render_tilewidth,
                             y_pos * render_tileheight, render_tilewidth,
                             render_tileheight};
        SDL_RenderCopy(renderer, tilesheet, &tile, &tile_pos);
      }
    }
  }
}

Map make_map_from_tmx(const char* tmx_path) {
  using namespace tinyxml2;
  Map map;
  XMLDocument map_xml;

  // Load tmx
  printf("loading tmx: %s\n",
         map_xml.LoadFile(tmx_path) == XML_SUCCESS ? "done" : "fail");

  // Extract metadata
  XMLElement* root = map_xml.FirstChildElement("map");
  map.width = atoi(root->Attribute("width"));
  map.height = atoi(root->Attribute("height"));
  map.tilewidth = atoi(root->Attribute("tilewidth"));
  map.tileheight = atoi(root->Attribute("tileheight"));

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
  map.layers = layers;
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
    printf("%d ", input.at(i));
  }
  printf("\n");
}

void print_2D_int_vector(int_vector_2D const input) {
  for (unsigned long i = 0; i < input.size(); i++) {
    print_1D_int_vector(input.at(i));
  }
}
