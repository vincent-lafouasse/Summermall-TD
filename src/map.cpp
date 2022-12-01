#include "map.h"

Map make_map_from_tmx(char* tmx_path) {
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
  char line_delim[] = "\n";
  char* line = NULL;
  output.clear();

  line = strtok(csv_string, line_delim);
  while (line != NULL) {
    output.push_back(vector_1D_from_string_line(line));
    line = strtok(NULL, line_delim);
  }

  return output;
}

int_vector_1D vector_1D_from_string_line(char* line_string) {
  int_vector_1D output;
  char cell_delim[] = ",";
  char* cell = NULL;
  output.clear();

  cell = strtok(line_string, cell_delim);
  while (cell != NULL) {
    output.push_back(atoi(cell));
    cell = strtok(NULL, cell_delim);
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
