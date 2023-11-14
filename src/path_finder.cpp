#include "path_finder.h"
#include <stdio.h>
#include <array>
#include <queue>

void DistanceField::reset() {
  reset(m_map);
}

void DistanceField::reset(const Map* map) {
  m_map = map;
  elements.clear();
  for (int row_idx = 0; row_idx < map->shape_tl.h; row_idx++) {
    std::vector<distance_t> row;
    for (int col_idx = 0; col_idx < map->shape_tl.w; col_idx++) {
      row.push_back(UNKNOWN_DISTANCE);
    }
    elements.push_back(row);
  }
}

bool DistanceField::try_computing_BFS(std::vector<Tower>* towers,
                                      Position entrance,
                                      Position exit) {
  if (m_map == nullptr) {
    return false;
  }

  set_at(exit, 0);

  std::queue<Position> queue;
  queue.push(exit);

  while (!queue.empty()) {
    Position current = queue.front();
    distance_t current_distance = at(current);
    queue.pop();
    std::array<Position, 4> neighbors =
        m_map->neighboring_tiles(current, towers);

    for (size_t i = 0; i < neighbors.size(); i++) {
      Position candidate = neighbors[i];
      if (candidate == INVALID_POSITION)
        continue;
      if (at(candidate) == UNKNOWN_DISTANCE) {
        set_at(candidate, current_distance + 1);
        queue.push(candidate);
      }
    }
  }
  return at(entrance) != UNKNOWN_DISTANCE;
}

void DistanceField::set_at(Position position, distance_t distance) {
  elements[position.y][position.x] = distance;
}

distance_t DistanceField::at(Position position) const {
  return elements[position.y][position.x];
}

void DistanceField::print(void) const {
  for (size_t row_idx = 0; row_idx < elements.size(); row_idx++) {
    std::vector<distance_t> row = elements[row_idx];
    for (size_t col_idx = 0; col_idx < row.size(); col_idx++) {
      distance_t element = row[col_idx];
      if (element != UNKNOWN_DISTANCE) {
        printf("%03i ", element);
      } else {
        printf("... ");
      }
    }
    printf("\n");
  }
}

bool DistanceField::min_neighbour(std::vector<Tower>* towers,
                                  Position position_tl,
                                  Position* next_position_tl) const {
  std::array<Position, 4> neighbours =
      m_map->neighboring_tiles(position_tl, towers);
  if (neighbours[0] == INVALID_POSITION) {
    return false;
  }

  int min_distance = elements[neighbours[0].y][neighbours[0].x];
  Position arg_min_distance = neighbours[0];
  for (Position neighbour : neighbours) {
    if (neighbour == INVALID_POSITION)
      continue;
    if (at(neighbour) < min_distance) {
      min_distance = at(neighbour);
      arg_min_distance = neighbour;
    }
  }

  if (next_position_tl != nullptr) {
    *next_position_tl = arg_min_distance;
  }
  return true;
}
