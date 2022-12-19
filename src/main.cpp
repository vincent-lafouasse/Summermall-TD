#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include "map.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 1440
#define SCREEN_WIDTH_DEV 400
#define SCREEN_HEIGHT_DEV 720
#define FPS_TARGET_FPS 60.

/*
  Returns the estimated FPS which is regulated to not exceed `FPS_TARGET_FPS`.

  @param tick_start The tick given by `SDL_GetTicks()` at the start of game
  loop.
  @return The current regulated FPS estimate.
*/
float fps_regulate_fps(Uint32 tick_start);

SDL_Rect get_tile_from_id(int id,
                          int tilesheet_width,
                          unsigned int tilewidth,
                          unsigned int tileheight) {
  int actual_id = id - 1;
  int X = (actual_id % tilesheet_width) * tilewidth;
  int Y = (actual_id / tilesheet_width) * tileheight;
  SDL_Rect tile = {X, Y, (int)tilewidth, (int)tileheight};
  return tile;
}

int main(void) {
  // Set up
  SDL_Window* window = SDL_CreateWindow(
      "Summermall TD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH_DEV, SCREEN_HEIGHT_DEV, SDL_WINDOW_OPENGL);

  if (window == nullptr) {
    SDL_Log("Could not create a window: %s", SDL_GetError());
    return -1;
  }

  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    SDL_Log("Could not create a renderer: %s", SDL_GetError());
    return -1;
  }

  // Load and parse map
  const char* basic_1P_tmx_path = "assets/maps/basic_1P.tmx";
  Map basic_1P_map = make_map_from_tmx(basic_1P_tmx_path);
  int tilewidth = basic_1P_map.tilewidth / 2;
  int tileheight = basic_1P_map.tileheight / 2;

  // Load tilesheet
  const char* tilesheet_path =
      "assets/tower-defense-top-down/Tilesheet/towerDefense_tilesheet.png";
  SDL_Texture* tilesheet =
      SDL_CreateTextureFromSurface(renderer, IMG_Load(tilesheet_path));
  unsigned int tilesheet_width = 1472 / basic_1P_map.tilewidth;

  // Game loop
  while (true) {
    Uint32 tick_start = SDL_GetTicks();

    // Get the next event
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }

    // Set the color to cornflower blue and clear
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);

    // Render map
    int current_layer_index = 0;
    int_vector_2D layer = basic_1P_map.layers.at(current_layer_index);

    for (int y_pos = 0; y_pos < (int)layer.size(); y_pos++) {
      int_vector_1D current_line = layer.at(y_pos);
      for (int x_pos = 0; x_pos < (int)current_line.size(); x_pos++) {
        SDL_Rect tile =
            get_tile_from_id(current_line.at(x_pos), tilesheet_width,
                             basic_1P_map.tilewidth, basic_1P_map.tileheight);
        SDL_Rect tile_position = {x_pos * tilewidth, y_pos * tileheight,
                                  tilewidth, tileheight};

        SDL_RenderCopy(renderer, tilesheet, &tile, &tile_position);
      }
    }

    // Show the renderer contents
    SDL_RenderPresent(renderer);

    // Compute and regulate fps to FPS_TARGET_FPS
    float fps = fps_regulate_fps(tick_start);
    if (0) {
      printf("%f", fps);
    }
  }
  // End of game loop

  // Tear down
  SDL_DestroyTexture(tilesheet);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

float fps_regulate_fps(Uint32 tick_start) {
  Uint32 tick_end = SDL_GetTicks();

  // Edge case where game loop runs in less than a 1ms
  if (tick_start == tick_end) {
    SDL_Delay(1);
    tick_end = SDL_GetTicks();
  }

  // FPS computation and regulation
  float ms_per_frame = (float)(tick_end - tick_start);
  float frame_per_s = 1 / (ms_per_frame / 1000.);

  if (frame_per_s > FPS_TARGET_FPS) {
    float ms_to_wait = ((1. / FPS_TARGET_FPS) - (1. / frame_per_s)) * 1000;
    SDL_Delay(ms_to_wait);
  }

  tick_end = SDL_GetTicks();

  ms_per_frame = (float)(tick_end - tick_start);
  frame_per_s = 1 / (ms_per_frame / 1000.);

  return frame_per_s;
}
