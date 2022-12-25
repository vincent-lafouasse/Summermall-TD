#include "render_map.h"
#include "render.h"

void render_map(const Map& map,
                SDL_Texture* tilesheet,
                const Rectangle dst_tileshape,
                SDL_Renderer* renderer) {
  for (size_t layer_index = 0; layer_index < map.layers.size(); layer_index++) {
    int_vector_2D layer = map.layers[layer_index];

    for (int y_pos = 0; y_pos < (int)layer.size(); y_pos++) {
      for (int x_pos = 0; x_pos < (int)layer[y_pos].size(); x_pos++) {
        SDL_Rect src_tile = make_tile_from_id(layer[y_pos][x_pos],
                                              map.src_tileshape, tilesheet);
        SDL_Rect dst_tile = {x_pos * dst_tileshape.w, y_pos * dst_tileshape.h,
                             dst_tileshape.w, dst_tileshape.h};
        SDL_RenderCopy(renderer, tilesheet, &src_tile, &dst_tile);
      }
    }
  }
}