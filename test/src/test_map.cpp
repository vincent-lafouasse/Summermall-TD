#include <gtest/gtest.h>
#include "map.h"

const char* BASIC_1P_MAP_PATH = "assets/maps/basic_1P.tmx";

TEST(Map, Metadata) {
  Map make_map_from_tmx(BASIC_1P_MAP_PATH);
}
