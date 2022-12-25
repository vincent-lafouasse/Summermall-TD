#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"

// Renders map on screen.
//
// @param map           A representation of the map to be rendered on screen.
// @param tilesheet     The tilesheet from which to extract the tiles.
// @param dst_tileshape The onscreen width and height of the tiles.
// @param rendere       The object in charge of rendering textures.
void render_map(const Map map,
                SDL_Texture* tilesheet,
                const Rectangle dst_tileshape,
                SDL_Renderer* renderer);
