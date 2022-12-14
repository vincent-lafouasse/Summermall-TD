#include <gtest/gtest.h>
#include "map.h"

const char* BASIC_1P_MAP_PATH = "assets/maps/basic_1P.tmx";

TEST(Map, Metadata) {
  Map map = make_map_from_tmx(BASIC_1P_MAP_PATH);
  unsigned int expected_tilewidth = 32;

  ASSERT_EQ(expected_tilewidth, map.tilewidth);
}
