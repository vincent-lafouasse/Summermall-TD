#include <SDL2/SDL.h>
#include "geometry.h"

class Monster {
 public:
  Position m_position;
  float m_orientation;

 private:
  SDL_Texture* m_texture;
  Rectangle m_shape;

 public:
  Monster(Position position,
          float orientation,
          const Rectangle dst_shape,
          SDL_Texture* texture);
  void render(SDL_Renderer* renderer);
  void move_by(int delta_x, int delta_y);
};


