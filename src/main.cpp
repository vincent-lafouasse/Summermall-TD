#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include "enemy.h"
#include "map.h"
#include "path_finder.h"
#include "render.h"
#include "render_map.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 1440
#define SCREEN_X_POS 0
#define SCREEN_Y_POS 0
#define FPS_TARGET_FPS 60.

// Regulate the fps to not exceed `FPS_TARGET_FPS` and return its estimated
// value.
//
// @param tick_start The tick given by `SDL_GetTicks()` at the start of game
// loop.
// @return The current regulated FPS estimate.
int fps_regulate_fps(Uint32 tick_start);

int main(void) {
  // Set up
  SDL_Window* window =
      SDL_CreateWindow("Summermall TD", SCREEN_X_POS, SCREEN_Y_POS,
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

  // Load tilesheet
  const char* tilesheet_path =
      "assets/tower-defense-top-down/Tilesheet/towerDefense_tilesheet.png";
  SDL_Texture* tilesheet =
      SDL_CreateTextureFromSurface(renderer, IMG_Load(tilesheet_path));

  // Load map
  const char* basic_1P_obstacles_tmx_path =
      "assets/maps/basic_1P_obstacles.tmx";
  const Map basic_1P_obstacles_map =
      parse_map_from_tmx(basic_1P_obstacles_tmx_path);

  const Dimension tileshape = basic_1P_obstacles_map.src_tileshape;

  SDL_Texture* static_map_texture = make_static_map_texture(
      &basic_1P_obstacles_map, tilesheet, tileshape, renderer);

  // Hardcoded waypoints
  Position checkpoint1 = pixel_pos_from_grid({13, 1}, tileshape);
  Position checkpoint2 = pixel_pos_from_grid({13, 23}, tileshape);
  Position corner1 = pixel_pos_from_grid({20, 3}, tileshape);
  Position corner2 = pixel_pos_from_grid({20, 5}, tileshape);
  Position corner3 = pixel_pos_from_grid({5, 6}, tileshape);
  Position corner4 = pixel_pos_from_grid({5, 8}, tileshape);
  Position corner5 = pixel_pos_from_grid({20, 9}, tileshape);
  Position corner6 = pixel_pos_from_grid({20, 11}, tileshape);

  std::vector<Position> hardcoded_waypoints{
      checkpoint1, corner1, corner2, corner3,
      corner4,     corner5, corner6, checkpoint2,
  };
  WaypointGraph graph;
  graph.add_vertices(&hardcoded_waypoints);
  graph.add_edge(checkpoint1, corner1);
  graph.add_edge(corner1, corner2);
  graph.add_edge(corner2, corner3);
  graph.add_edge(corner3, corner4);
  graph.add_edge(corner4, corner5);
  graph.add_edge(corner5, corner6);
  graph.add_edge(checkpoint2, corner6);

  // bad waypoints to test path finder
  Position node1 = pixel_pos_from_grid({4, 1}, tileshape);
  hardcoded_waypoints.push_back(node1);
  graph.add_edge(node1, checkpoint1);
  graph.add_edge(node1, corner1);
  Position node2 = pixel_pos_from_grid({19, 0}, tileshape);
  hardcoded_waypoints.push_back(node2);
  graph.add_edge(node2, checkpoint1);
  graph.add_edge(node2, corner1);
  graph.add_edge(node2, node1);
  Position node3 = pixel_pos_from_grid({4, 3}, tileshape);
  hardcoded_waypoints.push_back(node3);
  graph.add_edge(node3, node1);
  graph.add_edge(node3, corner1);
  Position node4 = pixel_pos_from_grid({21, 3}, tileshape);
  hardcoded_waypoints.push_back(node4);
  graph.add_edge(node4, checkpoint1);
  graph.add_edge(node4, corner1);
  Position node5 = pixel_pos_from_grid({7, 5}, tileshape);
  hardcoded_waypoints.push_back(node5);
  graph.add_edge(node5, corner2);
  graph.add_edge(node5, corner3);
  Position node6 = pixel_pos_from_grid({15, 6}, tileshape);
  hardcoded_waypoints.push_back(node6);
  graph.add_edge(node6, corner2);
  graph.add_edge(node6, corner3);
  Position node7 = pixel_pos_from_grid({9, 9}, tileshape);
  hardcoded_waypoints.push_back(node7);
  graph.add_edge(node7, corner4);
  graph.add_edge(node7, corner5);
  Position node8 = pixel_pos_from_grid({5, 14}, tileshape);
  hardcoded_waypoints.push_back(node8);
  graph.add_edge(node8, corner6);
  graph.add_edge(node8, checkpoint2);
  Position node9 = pixel_pos_from_grid({13, 11}, tileshape);
  hardcoded_waypoints.push_back(node9);
  graph.add_edge(node9, corner6);
  graph.add_edge(node9, checkpoint2);

  std::vector<Position> hardcoded_path{
      checkpoint1, corner1, corner2, corner3,
      corner4,     corner5, corner6, checkpoint2,
  };

  // Load mob texture
  const char* basic_mob_path =
      "assets/tower-defense-top-down/PNG/Default size/towerDefense_tile245.png";
  SDL_Texture* basic_mob_texture =
      SDL_CreateTextureFromSurface(renderer, IMG_Load(basic_mob_path));

  const Dimension mob_shape = tileshape;
  Position mob_position = checkpoint1;

  Monster monster(mob_position, mob_shape, basic_mob_texture);

  // Game loop -----------------------------------------------------------------
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

    // Set the color to red
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render map
    SDL_RenderCopy(renderer, static_map_texture, NULL, NULL);

    // Show corners and checkpoints
    highlight_points(&hardcoded_waypoints, 2, renderer);

    graph.render(renderer);

    // render mob
    if (!monster.m_reached_end) {
      monster.follow_path(&hardcoded_path);
      monster.render(renderer);
    }

    // Show the renderer contents
    SDL_RenderPresent(renderer);

    // Regulate fps to FPS_TARGET_FPS and estimate its actual value
    fps = fps_regulate_fps(tick_start);
  }
  // End of game loop ----------------------------------------------------------

  // Tear down
  SDL_DestroyTexture(tilesheet);
  SDL_DestroyTexture(static_map_texture);
  SDL_DestroyTexture(basic_mob_texture);
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
