#include <gtest/gtest.h>
#include "map.h"

const char* BASIC_1P_MAP_PATH = "assets/maps/basic_1P.tmx";
Map map = parse_map_from_tmx(BASIC_1P_MAP_PATH);

TEST(Basic1PMap, Metadata) {
  int expected_tilewidth = 32;
  int expected_tileheight = 32;
  int expected_width = 25;
  int expected_height = 45;

  ASSERT_EQ(expected_tilewidth, map.src_tileshape.w);
  ASSERT_EQ(expected_tileheight, map.src_tileshape.h);
  ASSERT_EQ(expected_width, map.shape.w);
  ASSERT_EQ(expected_height, map.shape.h);
}

TEST(Basic1PMap, LayersSize) {
  size_t expected_n_layers = 2;
  ASSERT_EQ(expected_n_layers, map.layers.size());

  int_vector_3D layers = map.layers;

  for (int i = 0; i < (int)layers.size(); i++) {
    int_vector_2D layer = layers.at(i);
    ASSERT_EQ(layer.size(), (size_t)map.shape.h);

    for (int j = 0; j < map.shape.h; j++) {
      ASSERT_EQ(layer.at(j).size(), (size_t)map.shape.w);
    }
  }
}
