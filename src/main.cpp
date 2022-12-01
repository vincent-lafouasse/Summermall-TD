#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>
#include "map.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS_TARGET_FPS 60.

/*
  Returns the estimated FPS which is regulated to not exceed `FPS_TARGET_FPS`.

  @param tick_start The tick given by `SDL_GetTicks()` at the start of game
  loop.
  @return The current regulated FPS estimate.
*/
float fps_regulate_fps(Uint32 tick_start);

int main(void) {
  // Set up
  SDL_Window* window = SDL_CreateWindow(
      "Summermall TD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

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

  // DEBUGGING
  printf("Testing print_vector_1D\n");
  int_vector_1D foo{1, 2, 3};
  print_1D_int_vector(foo);
  int_vector_1D bar{4, 5, 6};
  print_1D_int_vector(bar);
  printf("\n");

  printf("Testing print_vector_2D\n");
  int_vector_2D baz{foo, bar};
  print_2D_int_vector(baz);
  printf("\n");

  // Load and parse map
  Map map = make_map_from_tmx((char*)"assets/maps/basic_1P.tmx");

  printf("\nlayer 0\n");
  printf("length of layers[0] = %lu, should be %i\n", map.layers.at(0).size(),
         map.height);
  print_2D_int_vector(map.layers.at(0));

  printf("\nlayer 1\n");
  printf("length of layers[1] = %lu, should be %i\n", map.layers.at(1).size(),
         map.height);
  print_2D_int_vector(map.layers.at(1));

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
