#include "render.h"
#include <stdint.h>

std::vector<std::vector<Position>> make_polygon_outline(
    std::vector<Position> polygon) {
  std::vector<std::vector<Position>> polygon_edges;
  for (size_t i = 0; i < polygon.size() - 1; i++) {
    std::vector<Position> edge =
        get_Bresenham_line_between(polygon[i], polygon[i + 1]);
    polygon_edges.push_back(edge);
  }
  polygon_edges.push_back(
      get_Bresenham_line_between(polygon.front(), polygon.back()));
  return polygon_edges;
}

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

void set_render_color(Color color, SDL_Renderer* renderer) {
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
  uint8_t alpha = 255;
  switch (color) {
    case Color::BLACK: {
      break;
    }
    case Color::RED: {
      red = 255;
      break;
    }
    case Color::BLUE: {
      blue = 255;
      break;
    }
  }
  SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
}

Dimension get_texture_shape(SDL_Texture* texture) {
  int width, height;
  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  Dimension shape = {width, height};
  return shape;
}

void highlight_point(Position point, int width, SDL_Renderer* renderer) {
  SDL_Rect rect_around_point = {point.x - width, point.y - width, 2 * width,
                                2 * width};
  SDL_RenderDrawRect(renderer, &rect_around_point);
}

void highlight_points(std::vector<Position>* points,
                      int width,
                      SDL_Renderer* renderer) {
  for (size_t i = 0; i < points->size(); i++) {
    highlight_point(points->at(i), width, renderer);
  }
}

void render_point(Position position, SDL_Renderer* renderer) {
  SDL_Rect point = {position.x, position.y, 1, 1};
  SDL_RenderDrawRect(renderer, &point);
}

void highlight_tile(Position position,
                    Dimension tileshape,
                    SDL_Renderer* renderer) {
  SDL_Rect rect = {position.x, position.y, tileshape.w, tileshape.h};
  SDL_RenderDrawRect(renderer, &rect);
}

void render_vector(std::vector<Position>* vec, SDL_Renderer* renderer) {
  for (size_t i = 0; i < vec->size(); i++) {
    render_point(vec->at(i), renderer);
  }
}

void render_path(std::vector<std::vector<Position>>* path,
                 SDL_Renderer* renderer) {
  for (size_t i = 0; i < path->size(); i++) {
    render_vector(&(path->at(i)), renderer);
  }
}

void render_cursor(Position position,
                   Dimension shape,
                   SDL_Texture* texture,
                   SDL_Renderer* renderer) {
  SDL_Rect dst_rect = {position.x, position.y, shape.w, shape.h};
  SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
}

SDL_Texture* make_static_map_texture(const Map* map,
                                     SDL_Texture* tilesheet,
                                     const Dimension dst_tileshape,
                                     SDL_Renderer* renderer) {
  // Set shape of rendered map
  int width = map->shape_tl.w * dst_tileshape.w;
  int height = map->shape_tl.h * dst_tileshape.h;

  // Create  writable texture
  SDL_Texture* static_map_texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, width, height);
  if (static_map_texture == NULL) {
    printf("Unable to create static_map_texture, %s\n", SDL_GetError());
  }

  // Set target texture as render target
  if (SDL_SetRenderTarget(renderer, static_map_texture) != 0) {
    printf("Unable to set static_map_texture as render target, %s\n",
           SDL_GetError());
  }

  // Render map on static_map_texture
  render_map(map, tilesheet, dst_tileshape, renderer);

  // Set render target back to renderer
  if (SDL_SetRenderTarget(renderer, NULL) != 0) {
    printf(
        "unable to stop rendering to a texture and render to the window again, "
        "%s\n",
        SDL_GetError());
  }

  return static_map_texture;
}

void render_map(const Map* map,
                SDL_Texture* tilesheet,
                const Dimension dst_tileshape,
                SDL_Renderer* renderer) {
  for (size_t layer_index = 0; layer_index < map->layers.size();
       layer_index++) {
    int_vector_2D layer = map->layers[layer_index];

    for (int y_pos = 0; y_pos < (int)layer.size(); y_pos++) {
      for (int x_pos = 0; x_pos < (int)layer[y_pos].size(); x_pos++) {
        SDL_Rect src_tile = make_tile_from_id(layer[y_pos][x_pos],
                                              map->src_tileshape, tilesheet);
        Position dst_position =
            pixel_pos_from_grid({x_pos, y_pos}, dst_tileshape);
        SDL_Rect dst_tile = {dst_position.x, dst_position.y, dst_tileshape.w,
                             dst_tileshape.h};
        SDL_RenderCopy(renderer, tilesheet, &src_tile, &dst_tile);
      }
    }
  }
}
