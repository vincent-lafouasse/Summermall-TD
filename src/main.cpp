#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include "enemy.h"
#include "map.h"
#include "path_finder.h"
#include "render.h"
#include "render_map.h"
#include "tower.h"

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

bool are_connected(Tower tower1, Tower tower2, Dimension tower_shape) {
  int delta_x = tower1.m_position.x - tower2.m_position.x;
  int delta_y = tower1.m_position.y - tower2.m_position.y;
  delta_x = abs(delta_x);
  delta_y = abs(delta_y);
  return (delta_x <= tower_shape.w && delta_y <= tower_shape.h);
}

std::vector<std::set<Tower>> find_connected_towers(std::vector<Tower>* towers,
                                                   Dimension tower_shape);

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
  const char* tilesheet_png_path =
      "assets/tower-defense-top-down/Tilesheet/towerDefense_tilesheet.png";
  SDL_Texture* tilesheet =
      SDL_CreateTextureFromSurface(renderer, IMG_Load(tilesheet_png_path));

  // Load map
  const char* basic_1P_map_tmx_path = "assets/maps/basic_1P.tmx";
  const Map basic_1P_map = parse_map_from_tmx(basic_1P_map_tmx_path);

  const Map map = basic_1P_map;

  const Dimension tileshape = map.src_tileshape;
  const Dimension map_shape_tl = map.shape;
  // const Dimension map_shape = pixel_shape_from_grid(map_shape_tl, tileshape);

  SDL_Texture* static_map_texture =
      make_static_map_texture(&map, tilesheet, tileshape, renderer);

  // Load towers
  const char* block_tower_png_path =
      "assets/tower-defense-top-down/PNG/Default size/towerDefense_tile180.png";
  SDL_Texture* block_tower_texture =
      SDL_CreateTextureFromSurface(renderer, IMG_Load(block_tower_png_path));
  const int tower_size_tl = 2;
  const Dimension tower_shape = tileshape * tower_size_tl;
  std::vector<Tower> towers;

  for (int i = 0; i < 8; ++i) {
    Position tower_pos1 = pixel_pos_from_grid({4, 3}, tileshape);
    tower_pos1.x += i * tower_shape.w;
    Tower tower1(tower_pos1, tower_shape, block_tower_texture);
    towers.push_back(tower1);
  }
  for (int i = 0; i < 8; ++i) {
    Position tower_pos2 = pixel_pos_from_grid({5, 6}, tileshape);
    tower_pos2.x += i * tower_shape.w;
    Tower tower2(tower_pos2, tower_shape, block_tower_texture);
    towers.push_back(tower2);
  }
  for (int i = 0; i < 8; ++i) {
    Position tower_pos3 = pixel_pos_from_grid({4, 9}, tileshape);
    tower_pos3.x += i * tower_shape.w;
    Tower tower3(tower_pos3, tower_shape, block_tower_texture);
    towers.push_back(tower3);
  }
  for (size_t i = 0; i < towers.size(); i++) {
    towers[i].m_position.print();
  }
  for (size_t i = 0; i < towers.size(); i++) {
    size_t index = 1;
    printf("are tower %lu and %lu connected ? %s\n", index, i,
           are_connected(towers[index], towers[i], tower_shape) ? "yes" : "no");
  }

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
  graph.add_edge(node4, corner2);
  Position node5 = pixel_pos_from_grid({7, 5}, tileshape);
  hardcoded_waypoints.push_back(node5);
  graph.add_edge(node5, corner2);
  graph.add_edge(node5, corner3);
  Position node6 = pixel_pos_from_grid({15, 6}, tileshape);
  hardcoded_waypoints.push_back(node6);
  graph.add_edge(node6, corner2);
  graph.add_edge(node6, corner3);
  graph.add_edge(node6, node5);
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
  graph.add_edge(node9, node8);

  std::vector<Position> hardcoded_path{
      checkpoint1, corner1, corner2, corner3,
      corner4,     corner5, corner6, checkpoint2,
  };
  std::vector<std::vector<Position>> hardcoded_path_repr =
      get_path_repr(&hardcoded_path);

  // Dijkstra path finding
  std::vector<Position> dijkstra_path =
      Dijkstra_shortest_path(&graph, checkpoint1, checkpoint2);
  std::vector<std::vector<Position>> dijkstra_path_repr =
      get_path_repr(&dijkstra_path);

  // Load mob texture
  const char* basic_mob_png_path =
      "assets/tower-defense-top-down/PNG/Default size/towerDefense_tile245.png";
  SDL_Texture* basic_mob_texture =
      SDL_CreateTextureFromSurface(renderer, IMG_Load(basic_mob_png_path));

  const Dimension mob_shape = tileshape;
  Position mob_position = checkpoint1;

  Monster monster(mob_position, mob_shape, basic_mob_texture);

  const char* cursor_png_path =
      "assets/tower-defense-top-down/PNG/Default size/towerDefense_tile015.png";
  SDL_Texture* cursor_texture =
      SDL_CreateTextureFromSurface(renderer, IMG_Load(cursor_png_path));
  Position cursor_tl = {0, 0};
  Position cursor = pixel_pos_from_grid(cursor_tl, tileshape);
  const int cursor_size = 2;
  const Dimension cursor_shape_tl = {cursor_size, cursor_size};
  const Dimension cursor_shape =
      pixel_shape_from_grid(cursor_shape_tl, tileshape);

  int fps = 0;

  // Game loop -----------------------------------------------------------------
  bool is_running = true;
  bool show_graph = true;
  bool show_paths = true;
  while (is_running) {
    Uint32 tick_start = SDL_GetTicks();
    cursor = pixel_pos_from_grid(cursor_tl, tileshape);

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
            case SDLK_l:
              printf("cursor gridwise position: ");
              cursor_tl.print();
              printf("cursor pixel position: ");
              cursor.print();
              printf("\n");
              break;
            case SDLK_q: {
              if (can_put_tower_here(cursor, &towers, tower_shape)) {
                Tower tower(cursor, tower_shape, block_tower_texture);
                towers.push_back(tower);
              }
              break;
            }
            case SDLK_d: {
              delete_tower_at(cursor, &towers);
              break;
            }
            case SDLK_RIGHT:
              if ((cursor_tl.x + cursor_shape_tl.w) < map_shape_tl.w) {
                cursor_tl.x++;
              }
              break;
            case SDLK_LEFT:
              if (cursor_tl.x > 0) {
                cursor_tl.x--;
              }
              break;
            case SDLK_DOWN:
              if ((cursor_tl.y + cursor_shape_tl.h) < map_shape_tl.h) {
                cursor_tl.y++;
              }
              break;
            case SDLK_UP:
              if (cursor_tl.y > 0) {
                cursor_tl.y--;
              }
              break;
          }
          break;

        default:
          break;
      }
    }

    SDL_RenderClear(renderer);

    // Render map
    SDL_RenderCopy(renderer, static_map_texture, NULL, NULL);

    // Show graph in black
    if (show_graph) {
      set_render_color(Color::BLACK, renderer);
      highlight_points(&hardcoded_waypoints, 2, renderer);
      graph.render(renderer);
    }

    // Show hardcoded path in blue and computed path in red
    if (show_paths) {
      set_render_color(Color::BLUE, renderer);
      render_path(&hardcoded_path_repr, renderer);
      set_render_color(Color::RED, renderer);
      render_path(&dijkstra_path_repr, renderer);
    }

    // render mob
    if (!monster.m_reached_end) {
      monster.follow_path(&dijkstra_path);
      monster.render(renderer);
    }

    for (size_t i = 0; i < towers.size(); ++i) {
      (towers[i]).render(renderer);
    }
    render_cursor(cursor, cursor_shape, cursor_texture, renderer);
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
  SDL_DestroyTexture(block_tower_texture);
  SDL_DestroyTexture(cursor_texture);
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
