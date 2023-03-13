#include "map.h"
#include <string.h>
#include "third_party_lib/tinyxml2.h"

static int_vector_1D vector_1D_from_string_line(char* line_string);
static int_vector_2D vector_2D_from_string_csv(char* csv_string);

const Map parse_map_from_tmx(const char* tmx_path) {
  using namespace tinyxml2;
  XMLDocument map_xml;

  // Load tmx
  map_xml.LoadFile(tmx_path);

  // Extract metadata
  XMLElement* root = map_xml.FirstChildElement("map");
  const Dimension map_shape_tl = {atoi(root->Attribute("width")),
                                  atoi(root->Attribute("height"))};
  const Dimension map_src_tileshape = {atoi(root->Attribute("tilewidth")),
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
  std::set<Position> buildable_tiles;
  std::set<Position> traversable_tiles;
  read_buildability_and_traversability(&(layers[0]), &traversable_tiles,
                                       &buildable_tiles);
  Map map = {map_shape_tl, map_src_tileshape, layers, buildable_tiles,
             traversable_tiles};
  return map;
}

void read_buildability_and_traversability(
    int_vector_2D* ground_layer,
    std::set<Position>* return_traversable_tiles,
    std::set<Position>* return_buildable_tiles) {
  for (size_t row_idx = 0; row_idx < ground_layer->size(); row_idx++) {
    int_vector_1D row = ground_layer->at(row_idx);
    for (size_t column_idx = 0; column_idx < row.size(); column_idx++) {
      Position tile = {(int)column_idx, (int)row_idx};
      switch (row[column_idx]) {
        case GRASS:
          return_buildable_tiles->insert(tile);
          return_traversable_tiles->insert(tile);
          break;
        case SAND:
        case DIRT:
          return_traversable_tiles->insert(tile);
          break;
        default:
          break;
      }
    }
  }
}

static int_vector_2D vector_2D_from_string_csv(char* csv_string) {
  const char* line_delim = "\n";
  char* line;
  char* mem;
  int_vector_2D output;
  output.clear();

  line = strtok_r(csv_string, line_delim, &mem);
  while (line != NULL) {
    int_vector_1D res = vector_1D_from_string_line(line);
    output.push_back(res);
    line = strtok_r(NULL, line_delim, &mem);
  }
  return output;
}

static int_vector_1D vector_1D_from_string_line(char* line_string) {
  const char* cell_delim = ",";
  char* cell;
  char* mem;
  int_vector_1D output;
  output.clear();

  cell = strtok_r(line_string, cell_delim, &mem);
  while (cell != NULL) {
    output.push_back(atoi(cell));
    cell = strtok_r(NULL, cell_delim, &mem);
  }
  return output;
}
