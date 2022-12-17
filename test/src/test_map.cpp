#include <gtest/gtest.h>
#include "map.h"

const char* BASIC_1P_MAP_PATH = "assets/maps/basic_1P.tmx";
Map map = make_map_from_tmx(BASIC_1P_MAP_PATH);

TEST(Basic1PMap, Metadata) {
  unsigned int expected_tilewidth = 32;
  unsigned int expected_tileheight = 32;
  unsigned int expected_width = 25;
  unsigned int expected_height = 45;

  ASSERT_EQ(expected_tilewidth, map.tilewidth);
  ASSERT_EQ(expected_tileheight, map.tileheight);
  ASSERT_EQ(expected_width, map.width);
  ASSERT_EQ(expected_height, map.height);
}

TEST(Basic1PMap, LayersSize) {
  size_t expected_n_layers = 2;
  ASSERT_EQ(expected_n_layers, map.layers.size());

  size_t expected_height = 45;
  size_t expected_width = 25;

  for (size_t i = 0; i < map.layers.size(); i++) {
    int_vector_2D layer = map.layers.at(i);
    ASSERT_EQ(layer.size(), expected_height);

    for (size_t j = 0; j < layer.size(); j++) {
      ASSERT_EQ(layer.at(j).size(), expected_width);
    }
  }
}
