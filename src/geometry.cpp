#include "geometry.h"

Position pos_1D_to_2D(int pos_1D, int width) {
  return {pos_1D % width, pos_1D / width};
}