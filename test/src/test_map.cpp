#include <gtest/gtest.h>
#include "map.h"

const char* BASIC_1P_MAP_PATH = "assets/maps/basic_1P.tmx";

TEST(Basic1PMap, Metadata) {
  Map map = make_map_from_tmx(BASIC_1P_MAP_PATH);
  unsigned int expected_tilewidth = 32;
  unsigned int expected_tileheight = 32;
  unsigned int expected_width = 25;
  unsigned int expected_height = 45;

  ASSERT_EQ(expected_tilewidth, map.tilewidth);
  ASSERT_EQ(expected_tileheight, map.tileheight);
  ASSERT_EQ(expected_width, map.width);
  ASSERT_EQ(expected_height, map.height);
}
