#include "utils.h"

bool contains(const std::set<Position>* set, Position element) {
  auto search = set->find(element);
  return search != set->end();
}