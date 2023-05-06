#include "map.h"
#include <string.h>
#include "third_party_lib/tinyxml2.h"
#include "utils.h"

static int_vector_1D vector_1D_from_string_line(char* line_string);
static int_vector_2D vector_2D_from_string_csv(char* csv_string);

void read_checkpoints(int_vector_2D* checkpoint_layer,
                      std::vector<Position>* return_checkpoint_tiles) {
  for (size_t row_idx = 0; row_idx < checkpoint_layer->size(); row_idx++) {
    int_vector_1D row = checkpoint_layer->at(row_idx);
    for (size_t column_idx = 0; column_idx < row.size(); column_idx++) {
      if (row[column_idx] != 0) {
        Position checkpoint = Position(column_idx, row_idx);
        (*return_checkpoint_tiles).push_back(checkpoint);
      }
    }
  }
}

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
  std::vector<Position> checkpoint_tiles;
  read_checkpoints(&(layers[layers.size() - 1]), &checkpoint_tiles);
  Map map = {map_shape_tl,    map_src_tileshape, layers,
             buildable_tiles, traversable_tiles, checkpoint_tiles};
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

std::array<Position, 4> Map::neighboring_tiles(
    Position tile,
    std::vector<Tower>* towers) const {
  std::array<Position, 4> neighbors;
  for (auto& tile : neighbors)
    tile = INVALID_POSITION;
  size_t array_index = 0;

  std::vector<Position> candidates;
  candidates.push_back({tile.x + 1, tile.y});
  candidates.push_back({tile.x - 1, tile.y});
  candidates.push_back({tile.x, tile.y + 1});
  candidates.push_back({tile.x, tile.y - 1});

  for (size_t i = 0; i < candidates.size(); i++) {
    Position candidate = candidates[i];

    if (candidate.x < 0 || candidate.y < 0 || candidate.x >= shape_tl.w ||
        candidate.y >= shape_tl.h) {
      // ignore if out of bounds
      continue;
    }

    if (!contains(&(traversable_tiles), candidate)) {
      // ignore if tile is not in the set of traversable tiles
      continue;
    }

    bool is_valid = true;

    for (size_t i = 0; i < towers->size(); i++) {
      Tower tower = towers->at(i);
      std::set<Position> tiles_covered_by_tower;
      for (int x_inc = 0; x_inc < tower.m_size_tl; x_inc++) {
        for (int y_inc = 0; y_inc < tower.m_size_tl; y_inc++) {
          tiles_covered_by_tower.insert(Position(
              tower.m_position_tl.x + x_inc, tower.m_position_tl.y + y_inc));
        }
      }
      if (contains(&tiles_covered_by_tower, candidate)) {
        // ignore if within a towes
        is_valid = false;
        break;
      }
    }

    if (!is_valid) {
      continue;
    }

    neighbors[array_index] = candidate;
    array_index++;
  }
  return neighbors;
}
