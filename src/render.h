#pragma once

#include <SDL2/SDL.h>
#include "geometry.h"

// Give the position and shape of the tile numbered `tiled_id` in the tilesheet.
//
// @param tile_id       The integer encoding the tile.
// @param src_tileshape The width and height of the tiles in the tilesheet,
//                      i.e. how the tilesheet is to be subdivided.
// @param tilesheet     The `SDL_Texture` containing the tilesheet.
//
// @return A `SDL_Rect` containing the position and shape of the tile.
SDL_Rect make_tile_from_id(const int tile_id,
                           const Rectangle src_tileshape,
                           SDL_Texture* tilesheet);

// Return the width and height in pixels of parameter `texture`.
//
// @param texture The `SDL_Texture` to characterise.
//
// @return A Rectangle object containing the width and height of `texture`.
Rectangle get_texture_shape(SDL_Texture* texture);