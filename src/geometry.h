#pragma once

struct Rectangle {
  int w;
  int h;
};

struct Position {
  int x;
  int y;
};

// Return the 2D coordinates corresponding to the 1D coodinate in a rectangle of
// given width.
//
// @param pos_1D The 1D coordinate to convert.
// @param width The width of the rectangle in which the 1D coordinate is.
//
// @return A Rectangle object containing the width and height of `texture`.
Position pos_1D_to_2D(int pos_1D, int width);