#include "path_finder.h"
#include <stdio.h>
#include <queue>

bool DistanceField::try_computing_BFS(const Map* map,
                                      std::vector<Tower>* towers,
                                      Position entrance,
                                      Position exit) {
  init(map);
  set_at(exit, 0);

  std::queue<Position> queue;
  queue.push(exit);

  while (!queue.empty()) {
    Position current = queue.front();
    distance_t current_distance = at(current);
    queue.pop();
    std::vector<Position> neighbors = neighboring_tiles(current, map, towers);

    for (size_t i = 0; i < neighbors.size(); i++) {
      Position candidate = neighbors[i];
      if (at(candidate) == UNKNOWN_DISTANCE) {
        set_at(candidate, current_distance + 1);
        queue.push(candidate);
      }
    }
  }
  return at(entrance) != UNKNOWN_DISTANCE;
}

void DistanceField::init(const Map* map) {
  elements.clear();
  for (int row_idx = 0; row_idx < map->shape_tl.h; row_idx++) {
    std::vector<distance_t> row;
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

void DistanceField::set_at(Position position, distance_t distance) {
  elements[position.y][position.x] = distance;
}

distance_t DistanceField::at(Position position) {
  return elements[position.y][position.x];
}

void DistanceField::print(void) const {
  for (size_t row_idx = 0; row_idx < elements.size(); row_idx++) {
    std::vector<distance_t> row = elements[row_idx];
    for (size_t col_idx = 0; col_idx < row.size(); col_idx++) {
      printf("%03i ", row[col_idx]);
    }
    printf("\n");
  }
}
