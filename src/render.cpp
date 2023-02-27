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

void highlight_points(std::vector<Position>* points,
                      int width,
                      SDL_Renderer* renderer) {
  for (size_t i = 0; i < points->size(); i++) {
    SDL_Rect rect_around_point = {
        points->at(i).x - width, points->at(i).y - width, 2 * width, 2 * width};
    SDL_RenderDrawRect(renderer, &rect_around_point);
  }
}

void render_point(Position position, SDL_Renderer* renderer) {
  SDL_Rect point = {position.x, position.y, 1, 1};
  SDL_RenderDrawRect(renderer, &point);
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

void render_cursor(Position cursor,
                   Dimension cursor_shape_px,
                   SDL_Renderer* renderer) {
  SDL_Rect cursor_rect = {cursor.x * cursor_shape_px.w,
                          cursor.y * cursor_shape_px.h, cursor_shape_px.w,
                          cursor_shape_px.h};
  SDL_RenderDrawRect(renderer, &cursor_rect);
}
