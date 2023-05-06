#pragma once
#include <vector>

struct Dimension {
  int w;
  int h;

  Dimension(int w_ = 0, int h_ = 0) {
    w = w_;
    h = h_;
  }

  bool operator<(const Dimension& other) const;
  bool operator==(const Dimension& other) const;
  Dimension operator+(const Dimension& other) const;
  Dimension operator*(const int& scalar) const;
};

struct Position {
  int x;
  int y;

  Position(int x_ = 0, int y_ = 0) {
    x = x_;
    y = y_;
  }

  void print(void);
  bool operator<(const Position& other) const;
  bool operator>(const Position& other) const;
  bool operator==(const Position& other) const;
  bool operator!=(const Position& other) const;
  Position operator+(const Position& other) const;
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
Position grid_pos_from_pixel(Position position_px, Dimension tileshape);
Position pixel_pos_from_grid(Position position_tl, Dimension tileshape);
Dimension pixel_shape_from_grid(Dimension shape_tl, Dimension tileshape);

Position tile_center(Position tile, Dimension tileshape);

float line_angle(std::vector<Position>* line);

// Return the rasterized line between two Positions.
std::vector<Position> get_Bresenham_line_between(Position start, Position end);

bool line_is_vertical(Position start, Position end);
bool is_between(int candidate, int lower_bound, int upper_bound);
std::vector<Position> flatten_2D_position_vector(
    std::vector<std::vector<Position>>* vector);
std::vector<Position> append_position_vector(std::vector<Position>* vec1,
                                             std::vector<Position>* vec2);
