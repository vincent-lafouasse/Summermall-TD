#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include "tinyxml2.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS_TARGET_FPS 60.

struct Map {
  unsigned int width;
  unsigned int height;
  unsigned int tilewidth;
  unsigned int tileheight;
  std::vector<std::vector<std::vector<int>>> vec;
};

Map make_map_from_tmx(char* tmx_path);
std::vector<std::vector<int>> csv_string_to_vec(char* csv_string);

/*
  Returns the current FPS estimate which is regulated to fit
  `FPS_TARGET_FPS` in the case where its bigger.

  @param tick_start The tick given by `SDL_GetTicks()` at the start of
  game loop.
  @return The current regulated FPS estimate.
*/
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

  // Load and parse map
  Map map = make_map_from_tmx((char*)"assets/maps/basic_1P.tmx");
  printf("width = %i\n", map.width);
  printf("height = %i\n", map.height);
  printf("tilewidth = %i\n", map.tilewidth);
  printf("tileheight = %i\n", map.tileheight);

  while (true) {
    Uint32 tick_start = SDL_GetTicks();

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

    float fps = fps_regulate_fps(tick_start);
  }

  // Tidy up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

Map make_map_from_tmx(char* tmx_path) {
  Map map;
  tinyxml2::XMLDocument map_xml;
  tinyxml2::XMLError loaded = map_xml.LoadFile(tmx_path);
  printf("loading tmx: %s\n",
         loaded == tinyxml2::XML_SUCCESS ? "done" : "fail");

  tinyxml2::XMLElement* root = map_xml.FirstChildElement("map");
  map.width = atoi(root->Attribute("width"));
  map.height = atoi(root->Attribute("height"));
  map.tilewidth = atoi(root->Attribute("tilewidth"));
  map.tileheight = atoi(root->Attribute("tileheight"));

  tinyxml2::XMLElement* layer = root->FirstChildElement("layer");
  printf("layer id: %i\n", atoi(layer->Attribute("id")));
  printf("\n");

  tinyxml2::XMLElement* data = layer->FirstChildElement("data");
  printf("encoding: %s\n", data->Attribute("encoding"));
  printf("\n");

  const char* csv = data->GetText();
  printf("%s\n", csv);
  return map;
}

std::vector<std::vector<int>> csv_string_to_vec(char* csv_string) {
  std::vector<std::vector<int>> output;

  return output;
}
