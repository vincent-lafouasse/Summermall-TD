#include "map.h"

static int_vector_1D vector_1D_from_string_line(char* line_string);
static int_vector_2D vector_2D_from_string_csv(char* csv_string);

const Map parse_map_from_tmx(const char* tmx_path) {
  using namespace tinyxml2;
  XMLDocument map_xml;

  // Load tmx
  printf("loading tmx: %s\n",
         map_xml.LoadFile(tmx_path) == XML_SUCCESS ? "done" : "fail");

  // Extract metadata
  XMLElement* root = map_xml.FirstChildElement("map");
  const Dimension map_shape = {atoi(root->Attribute("width")),
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
  Map map = {map_shape, map_src_tileshape, layers};
  return map;
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
