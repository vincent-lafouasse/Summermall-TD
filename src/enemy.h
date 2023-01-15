#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "geometry.h"

///
// A monster that walks toward the exit.
//
// @member m_position    The world coordinates of the monster in pixel units.
//                       The monster will be rendered centered on that position.
// @member m_orientation The orientation of the monster in degrees. 0 means to
//                       left, i.e aligned with the vector (x = 1, y = 0). Goes
//                       clockwise.
// @member m_reached_end A boolean that tracks if the monster has reached the
//                       exit. If true, the monster should despawn and a life
//                       subtracted from the player's lives.
// @member m_texture     The static texture of the monster.
// @member m_shape       The onscreen dimension of the monster.
// @member m_step_id     The position of the monster on a given Bresenham line.
// @member m_edge_id     The edge of the path the monster is following.
//                       Indicates which waypoint the monster is headed to.
//
// @method Monster    Constructor. Assigns the arguments to the right members.
// @param position    Assigned to m_position.
// @param dst_shape   Assigned to m_shape.
// @param texture     Assigned to m_texture.
//
// @method follow_path Given a vector of waypoints, visit each waypoint in
//                     order. The waypoints are usually not adjacent. Also
//                     orients the monster toward the waypoint.
// @param path         A pointer to the vector containing all the waypoints to
//                     visit.
//
// @method follow_line Given a vector of positions, visit each position in
//                     order. Each of these position should be adjacent
//                     (orthogonally or diagonally).
// @param line         A pointer to the vector containing all the positions to
//                     visit.
//
// @method step_toward_simple Move toward a given position. Should only be used
//                            for adjacent positions or else the movement will
//                            look un-natural.
// @param destination         The position to move toward.
//
// @method move_by Shift the position of the monster.
// @param delta_x
// @param delta_y
//
// @method render  Render the monster onscreen (centered on m_position)
// @param renderer
class Monster {
 public:
  Position m_position;
  float m_orientation;
  bool m_reached_end;

 private:
  SDL_Texture* m_texture;
  Dimension m_shape;
  int m_step_id;
  int m_edge_id;

 public:
  Monster(Position position, const Dimension dst_shape, SDL_Texture* texture);
  void follow_path(std::vector<Position>* path);
  void follow_line(std::vector<Position>* line);
  void step_toward_simple(Position destination);
  void move_by(int delta_x, int delta_y);
  void render(SDL_Renderer* renderer);
};
