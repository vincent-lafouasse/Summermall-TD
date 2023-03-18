#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "geometry.h"
#include "map.h"

enum class Color { BLACK, RED, BLUE };

// Create a static image of the map that is easily rendered.
//
// @param map           A representation of the map to be rendered.
// @param tilesheet     The tilesheet from which to extract the tiles.
// @param dst_tileshape The onscreen width and height of the tiles.
// @param rendere       The object in charge of rendering textures.
SDL_Texture* make_static_map_texture(const Map* map,
                                     SDL_Texture* tilesheet,
                                     const Dimension dst_tileshape,
                                     SDL_Renderer* renderer);

// Renders map to render target
//
// @param map           A representation of the map to be rendered.
// @param tilesheet     The tilesheet from which to extract the tiles.
// @param dst_tileshape The onscreen width and height of the tiles.
// @param rendere       The object in charge of rendering textures.
void render_map(const Map* map,
                SDL_Texture* tilesheet,
                const Dimension dst_tileshape,
                SDL_Renderer* renderer);

void set_render_color(Color color, SDL_Renderer* renderer);

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

std::vector<std::vector<Position>> make_polygon_outline(
    std::vector<Position> polygon);
void highlight_point(Position point, int width, SDL_Renderer* renderer);
void highlight_points(std::vector<Position>* points,
                      int width,
                      SDL_Renderer* renderer);

void render_vector(std::vector<Position>* vec, SDL_Renderer* renderer);
void render_path(std::vector<std::vector<Position>>* path,
                 SDL_Renderer* rendere);
void render_cursor(Position position,
                   Dimension shape,
                   SDL_Texture* texture,
                   SDL_Renderer* renderer);
void highlight_tile(Position position,
                    Dimension tileshape,
                    SDL_Renderer* renderer);
