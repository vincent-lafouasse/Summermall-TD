#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS_INTERVAL_IN_MS 1000.0  // milliseconds
#define FPS_TARGET_FPS 60.

int main(int argc, char* args[]) {
  Uint32 fps_last_time_fps_was_computed = SDL_GetTicks();
  Uint32 fps_current_fps = 0;
  Uint32 fps_elapsed_frames_since_last_measure = 0;

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

    // FPS measure and cap
    fps_elapsed_frames_since_last_measure++;
    // FPS measure;
    if (SDL_GetTicks() - fps_last_time_fps_was_computed > FPS_INTERVAL_IN_MS) {
      // If more than 1000 ms has elapsed since fps_last_time_fps_was_computed
      // then compute estimated fps and reset fps_last_time[...] and
      // fps_elapsed_frames[...]
      fps_current_fps = fps_elapsed_frames_since_last_measure;
      fps_last_time_fps_was_computed = SDL_GetTicks();
      fps_elapsed_frames_since_last_measure = 0;
    }

    // FPS Cap: if fps is too high
    // wait for actual time/frame - target time/frame (in s)
    if (fps_current_fps > FPS_TARGET_FPS) {
      sleep((1. / FPS_TARGET_FPS) - (1. / (float)fps_current_fps));
    }
    printf("%i\n", fps_current_fps);
  }

  // Tidy up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
