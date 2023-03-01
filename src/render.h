#pragma once

#include <SDL2/SDL.h>
#include <vector>
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
                           const Dimension src_tileshape,
                           SDL_Texture* tilesheet);

// Return the width and height in pixels of parameter `texture`.
//
// @param texture The `SDL_Texture` to characterise.
//
// @return A Dimension object containing the width and height of `texture`.
Dimension get_texture_shape(SDL_Texture* texture);

void highlight_points(std::vector<Position>* points,
                      int width,
                      SDL_Renderer* renderer);

void render_vector(std::vector<Position>* vec, SDL_Renderer* renderer);
void render_path(std::vector<std::vector<Position>>* path,
                 SDL_Renderer* rendere);
