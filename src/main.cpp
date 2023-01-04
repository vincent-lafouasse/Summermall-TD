#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include "map.h"
#include "render_map.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 1440
#define ZOOM_DIVIDOR 2
#define FPS_TARGET_FPS 60.
#define SCREEN_X_POS 0
#define SCREEN_Y_POS 0

// Regulate the fps to not exceed `FPS_TARGET_FPS` and return its estimated
// value.
//
// @param tick_start The tick given by `SDL_GetTicks()` at the start of game
// loop.
// @return The current regulated FPS estimate.
int fps_regulate_fps(Uint32 tick_start);

int main(void) {
  // Set up
  SDL_Window* window = SDL_CreateWindow(
      "Summermall TD", SCREEN_X_POS, SCREEN_Y_POS, SCREEN_WIDTH / ZOOM_DIVIDOR,
      SCREEN_HEIGHT / ZOOM_DIVIDOR, SDL_WINDOW_OPENGL);

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

  // Load map
  const char* basic_1P_tmx_path = "assets/maps/basic_1P.tmx";
  const Map basic_1P_map = parse_map_from_tmx(basic_1P_tmx_path);

  // Load tilesheet
  const char* tilesheet_path =
      "assets/tower-defense-top-down/Tilesheet/towerDefense_tilesheet.png";
  SDL_Texture* tilesheet =
      SDL_CreateTextureFromSurface(renderer, IMG_Load(tilesheet_path));

  // Set size of tiles on screen
  const Rectangle tileshape = {basic_1P_map.src_tileshape.w / ZOOM_DIVIDOR,
                               basic_1P_map.src_tileshape.h / ZOOM_DIVIDOR};

  // Create map texture
  SDL_Texture* static_map_texture =
      make_static_map_texture(&basic_1P_map, tilesheet, tileshape, renderer);

  // a mob
  // with src_tileshape = {64, 64}, mob is at position X = 15 Y = 10
  Rectangle mob_src_shape = {64, 64};
  Position mob_src_position = {15, 10};
  SDL_Rect mob_in_tilesheet = {mob_src_position.x * mob_src_shape.w,
                               mob_src_position.y * mob_src_shape.h,
                               mob_src_shape.w, mob_src_shape.h};

	// set mob position to first checkpoint
  Rectangle mob_dst_shape = {mob_src_shape.w / ZOOM_DIVIDOR,
                             mob_src_shape.h / ZOOM_DIVIDOR};
  Position mob_dst_position = {6, 0};
  SDL_Rect mob_onscreen = {mob_dst_position.x * mob_dst_shape.w,
                           mob_dst_position.y * mob_dst_shape.h,
                           mob_dst_shape.w, mob_dst_shape.h};

  // Game loop
  bool is_running = true;
  int fps = 0;
  while (is_running) {
    Uint32 tick_start = SDL_GetTicks();

    // Get the next event
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          is_running = false;
          break;

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_p:
              printf("FPS: %i\n", fps);
              break;
          }
          break;

        default:
          break;
      }
    }

    // Set the color to cornflower blue and clear
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);

    // Render map
    SDL_RenderCopy(renderer, static_map_texture, NULL, NULL);

		// render mob
    SDL_RenderCopy(renderer, tilesheet, &mob_in_tilesheet, &mob_onscreen);

    // Show the renderer contents
    SDL_RenderPresent(renderer);

    // Regulate fps to FPS_TARGET_FPS and estimate its actual value
    fps = fps_regulate_fps(tick_start);
  }
  // End of game loop

  // Tear down
  SDL_DestroyTexture(tilesheet);
  SDL_DestroyTexture(static_map_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

int fps_regulate_fps(Uint32 tick_start) {
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

  return (int)frame_per_s;
}
