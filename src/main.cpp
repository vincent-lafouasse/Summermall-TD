#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include "tinyxml2.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS_TARGET_FPS 60.
typedef std::vector<int> int_vector_1D;
typedef std::vector<std::vector<int>> int_vector_2D;
typedef std::vector<std::vector<std::vector<int>>> int_vector_3D;

struct Map {
  unsigned int width;
  unsigned int height;
  unsigned int tilewidth;
  unsigned int tileheight;
  int_vector_3D layers;
};

Map make_map_from_tmx(char* tmx_path);
int_vector_2D vector_2D_from_string_csv(char* csv_string);
void print_1D_vector(int_vector_1D const input);
void print_2D_vector(int_vector_2D const input);

/*
  Returns the estimated FPS which is regulated to not exceed `FPS_TARGET_FPS`.

  @param tick_start The tick given by `SDL_GetTicks()` at the start of game
  loop.
  @return The current regulated FPS estimate.
*/
float fps_regulate_fps(Uint32 tick_start);

int main(int argc, char* args[]) {
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

  // Load and parse map
  Map map = make_map_from_tmx((char*)"assets/maps/basic_1P.tmx");
  printf("width = %i\n", map.width);
  printf("height = %i\n", map.height);
  printf("tilewidth = %i\n", map.tilewidth);
  printf("tileheight = %i\n", map.tileheight);

  int_vector_1D foo;
  int_vector_1D bar;
  int_vector_2D baz;
  for (int i = 0; i <= 5; ++i) {
    foo.push_back(i);
  }
  for (int i = 420; i <= 425; ++i) {
    bar.push_back(i);
  }
  baz.push_back(foo);
  baz.push_back(bar);
  print_2D_vector(baz);

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
  }
  // End of game loop

  // Tear down
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

Map make_map_from_tmx(char* tmx_path) {
  using namespace tinyxml2;
  Map map;
  XMLDocument map_xml;

  // Load tmx
  printf("loading tmx: %s\n",
         map_xml.LoadFile(tmx_path) == XML_SUCCESS ? "done" : "fail");

  // Extract metadata
  XMLElement* root = map_xml.FirstChildElement("map");
  map.width = atoi(root->Attribute("width"));
  map.height = atoi(root->Attribute("height"));
  map.tilewidth = atoi(root->Attribute("tilewidth"));
  map.tileheight = atoi(root->Attribute("tileheight"));

  // Extract map layers into 3D vector from csv
  XMLElement* layer = root->FirstChildElement("layer");
  char* layer_csv;
  int_vector_3D layers;
  while (layer) {
    printf("\n");
    printf("layer id: %i\n", atoi(layer->Attribute("id")));

    layer_csv = (char*)layer->FirstChildElement("data")->GetText();

    layers.push_back(vector_2D_from_string_csv(layer_csv));

    layer = layer->NextSiblingElement();
  }
  map.layers = layers;
  return map;
}

void print_1D_vector(int_vector_1D const input) {
  for (unsigned long i = 0; i < input.size(); i++) {
    printf("%d ", input.at(i));
  }
  printf("\n");
}

void print_2D_vector(int_vector_2D const input) {
  for (unsigned long i = 0; i < input.size(); i++) {
    print_1D_vector(input.at(i));
  }
}

int_vector_2D vector_2D_from_string_csv(char* csv_string) {
  std::vector<std::string> csv_lines;
  char delims[] = "\n";
  char* line = NULL;
  char* cell_item = NULL;

  line = strtok(csv_string, delims);
  while (line != NULL) {
    printf("%s\n", line);
    csv_lines.push_back(line);
    line = strtok(NULL, delims);
  }

  int_vector_2D output;
  return output;
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
