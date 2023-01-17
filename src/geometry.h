#pragma once
#include <vector>

struct Dimension {
  int w;
  int h;

  Dimension(int w_ = 0, int h_ = 0) {
    w = w_;
    h = h_;
  }

  bool operator<(const Dimension& other) const {
    return w < other.w || (w == other.w && h < other.h);
  }
  bool operator==(const Dimension& other) const {
    return (w == other.w) && (h == other.h);
  }
  Dimension operator+(const Dimension& other) const {
    return Dimension(w + other.w, h + other.h);
  }
};

struct Position {
  int x;
  int y;

  Position(int x_ = 0, int y_ = 0) {
    x = x_;
    y = y_;
  }

  bool operator<(const Position& other) const {
    return x < other.x || (x == other.x && y < other.y);
  }
  bool operator==(const Position& other) const {
    return (x == other.x) && (y == other.y);
  }
  Position operator+(const Position& other) const {
    return Position(x + other.x, y + other.y);
  }

  void print(void);
};

// Return the 2D coordinates corresponding to the 1D coodinate in a rectangle of
// a given width.
//
// @param pos_1D The 1D coordinate to convert.
// @param width  The width of the rectangle in which the 1D coordinate is.
//
// @return A Position object containing the 2D coordinates.
Position pos_1D_to_2D(int pos_1D, int width);
int pos_2D_to_1D(Position position, int width);
Position pixel_pos_from_grid(Position grid_pos, Dimension tileshape);

// Return the Euclidian distance between two Positions.
float euclidian_distance(Position a, Position b);

float line_angle(std::vector<Position>* line);

// Return the rasterized line between two Positions.
std::vector<Position> get_Bresenham_line_between(Position start, Position end);
