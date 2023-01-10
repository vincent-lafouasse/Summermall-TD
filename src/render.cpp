#include "render.h"

SDL_Rect make_tile_from_id(const int tile_id,
                           const Dimension src_tileshape,
                           SDL_Texture* tilesheet) {
  // The tilesheet is indexed in the following manner :
  // (with example n_of_tiles_per_row=4)
  // 1 2 3 4
  // 5 6 7 8 etc
  //
  // The tile representated by tile_id=7 is thus at position :
  // x = 2 = (7 - 1) % 4 and y = 1 = (7 - 1) // 4.
  // A tile_id of zero means the tile is empty/transparent.
  const int actual_id = tile_id - 1;
  const int n_of_tiles_per_row =
      get_texture_shape(tilesheet).w / src_tileshape.w;

  const Position pos_in_sheet = pos_1D_to_2D(actual_id, n_of_tiles_per_row);

  const int X = pos_in_sheet.x * src_tileshape.w;
  const int Y = pos_in_sheet.y * src_tileshape.h;
  const SDL_Rect tile = {X, Y, src_tileshape.w, src_tileshape.h};
  return tile;
}

Dimension get_texture_shape(SDL_Texture* texture) {
  int width, height;
  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  Dimension shape = {width, height};
  return shape;
}
