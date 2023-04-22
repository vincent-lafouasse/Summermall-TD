#include "path_finder.h"
#include <stdio.h>

void DistanceField::init(const Map* map) {
  for (int row_idx = 0; row_idx < map->shape_tl.h; row_idx++) {
    std::vector<alt_distance_t> row;
    for (int col_idx = 0; col_idx < map->shape_tl.w; col_idx++) {
      row.push_back(UNKNOWN_DISTANCE);
    }
    elements.push_back(row);
  }
}

static bool contains(const std::set<Position>* set, Position element) {
  auto search = set->find(element);
  return search != set->end();
}

std::vector<Position> neighboring_tiles(Position tile,
                                        const Map* map,
                                        std::vector<Tower>* towers) {
  std::vector<Position> candidates;
  candidates.push_back({tile.x + 1, tile.y});
  candidates.push_back({tile.x - 1, tile.y});
  candidates.push_back({tile.x, tile.y + 1});
  candidates.push_back({tile.x, tile.y - 1});

  std::vector<Position> neighbors;
  for (size_t i = 0; i < candidates.size(); i++) {
    Position candidate = candidates[i];

    if (candidate.x < 0 || candidate.y < 0 || candidate.x >= map->shape_tl.w ||
        candidate.y >= map->shape_tl.h) {
      // ignore if out of bounds
      continue;
    }

    if (!contains(&(map->traversable_tiles), candidate)) {
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

    neighbors.push_back(candidate);
  }
  return neighbors;
}

void DistanceField::print(void) const {
  for (size_t row_idx = 0; row_idx < elements.size(); row_idx++) {
    std::vector<alt_distance_t> row = elements[row_idx];
    for (size_t col_idx = 0; col_idx < row.size(); col_idx++) {
      printf("%i ", row[col_idx]);
    }
    printf("\n");
  }
}
