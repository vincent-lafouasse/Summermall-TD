#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS_TARGET_FPS 60.

int main(int argc, char* args[]) {
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

  while (true) {
    Uint32 start = SDL_GetTicks();

    // Get the next event
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        // Break out of the loop on quit
        break;
      }
    }

    // Set the color to cornflower blue and clear
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);
    // Show the renderer contents
    SDL_RenderPresent(renderer);

    // Otherwise, loop might take less than 1 ms
    // and ms_per_frame is 0
    SDL_Delay(1);

    Uint32 end = SDL_GetTicks();

    // FPS computation and regulation
    float ms_per_frame = (float)(end - start);
    float frame_per_s = 1 / (ms_per_frame / 1000.);

    if (frame_per_s > FPS_TARGET_FPS) {
      float ms_to_wait = ((1. / FPS_TARGET_FPS) - (1. / frame_per_s)) * 1000;
      SDL_Delay(ms_to_wait);
    }

    end = SDL_GetTicks();

    ms_per_frame = (float)(end - start);
    frame_per_s = 1 / (ms_per_frame / 1000.);

    printf("%f\n", frame_per_s);
  }

  //
  // Tidy up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
