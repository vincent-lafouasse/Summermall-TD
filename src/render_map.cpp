#include "render_map.h"
#include "render.h"

void render_map(const Map* map,
                SDL_Texture* tilesheet,
                const Rectangle dst_tileshape,
                SDL_Renderer* renderer) {
  for (size_t layer_index = 0; layer_index < map->layers.size();
       layer_index++) {
    int_vector_2D layer = map->layers[layer_index];

    for (int y_pos = 0; y_pos < (int)layer.size(); y_pos++) {
      for (int x_pos = 0; x_pos < (int)layer[y_pos].size(); x_pos++) {
        SDL_Rect src_tile = make_tile_from_id(layer[y_pos][x_pos],
                                              map->src_tileshape, tilesheet);
        SDL_Rect dst_tile = {x_pos * dst_tileshape.w, y_pos * dst_tileshape.h,
                             dst_tileshape.w, dst_tileshape.h};
        SDL_RenderCopy(renderer, tilesheet, &src_tile, &dst_tile);
      }
    }
  }
}

SDL_Texture* make_static_map_texture(const Map* map,
                                     SDL_Texture* tilesheet,
                                     const Rectangle dst_tileshape,
                                     SDL_Renderer* renderer) {
  // Set shape of rendered map
  int width = map->shape.w * dst_tileshape.w;
  int height = map->shape.h * dst_tileshape.h;

  // Create  writable texture
  SDL_Texture* static_map_texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, width, height);
  assert(static_map_texture != NULL && "static_map_texture cant be NULL");

  // Set target texture as render target
  if (SDL_SetRenderTarget(renderer, static_map_texture) != 0) {
    printf("unable to set static_map_texture as render target, %s\n",
           SDL_GetError());
  }

  // Set render target back to renderer
  if (SDL_SetRenderTarget(renderer, NULL) != 0) {
    printf(
        "unable to stop rendering to a texture and render to the window again, "
        "%s\n",
        SDL_GetError());
  }

  return static_map_texture;
}
