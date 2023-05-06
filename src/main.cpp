#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include <list>
#include "enemy.h"
#include "geometry.h"
#include "map.h"
#include "old_path_finder.h"
#include "path_finder.h"
#include "render.h"
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

// init window and renderer and handle some errors
void init_sdl(const Position screen_pos,
              const Dimension screen_shape,
              SDL_Window** return_window,
              SDL_Renderer** return_renderer);

// Construct hardcoded maze 0 (simple)
std::vector<Tower> setup_maze0(int tower_size_tl, SDL_Texture* tower_texture);
// Construct hardcoded maze 1 (non convex tower clusters)
std::vector<Tower> setup_maze1(int tower_size_tl, SDL_Texture* tower_texture);

// Construct hardcoded waypoint graph and path for maze 0
void get_hardcoded_graph_and_path_maze0(const Map* map,
                                        Dimension tileshape,
                                        WaypointGraph* return_graph,
                                        std::vector<Position>* return_path);

// Contain hardcoded paths to assets
SDL_Texture* load_mob_texture(SDL_Renderer* renderer);
SDL_Texture* load_block_tower_texture(SDL_Renderer* renderer);
SDL_Texture* load_tilesheet_texture(SDL_Renderer* renderer);
SDL_Texture* load_cursor_texture(SDL_Renderer* renderer);
Map load_basic_map(void);

int main(void) {
  // --------------SDL--------------------------------------
  const Position screen_position = {SCREEN_X_POS, SCREEN_Y_POS};
  const Dimension screen_shape = {SCREEN_WIDTH, SCREEN_HEIGHT};
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  init_sdl(screen_position, screen_shape, &window, &renderer);

  // --------------TILESHEET--------------------------------------
  SDL_Texture* tilesheet = load_tilesheet_texture(renderer);

  // --------------MAP--------------------------------------
  const Map basic_1P_map = load_basic_map();

  const Map map = basic_1P_map;

  const Dimension tileshape = map.src_tileshape;
  const Dimension map_shape_tl = map.shape_tl;

  SDL_Texture* static_map_texture =
      make_static_map_texture(&map, tilesheet, tileshape, renderer);

  // --------------TOWERS--------------------------------------
  SDL_Texture* block_tower_texture = load_block_tower_texture(renderer);
  const int tower_size_tl = 2;

  std::vector<Tower> towers = setup_maze0(tower_size_tl, block_tower_texture);

  // --------------WAYPOINTS--------------------------------------
  Position checkpoint1 = tile_center(
      pixel_pos_from_grid(map.checkpoint_tiles[0], tileshape), tileshape);
  Position checkpoint2 = tile_center(
      pixel_pos_from_grid(map.checkpoint_tiles[1], tileshape), tileshape);

  // --------------OLD PATH FINDING--------------------------------------
  std::vector<Position> hardcoded_path;
  WaypointGraph hardcoded_graph;
  get_hardcoded_graph_and_path_maze0(&map, tileshape, &hardcoded_graph,
                                     &hardcoded_path);
  std::vector<std::vector<Position>> hardcoded_path_repr =
      get_path_repr(&hardcoded_path);

  std::vector<Position> dijkstra_path =
      Dijkstra_shortest_path(&hardcoded_graph, checkpoint1, checkpoint2);
  std::vector<std::vector<Position>> dijkstra_path_repr =
      get_path_repr(&dijkstra_path);

  // --------------DISTANCE FIELD--------------------------------------
  DistanceField distance_field(&map);
  bool bfs_success = distance_field.try_computing_BFS(
      &towers, map.checkpoint_tiles[0], map.checkpoint_tiles[1]);
  distance_field.print();
  printf("BFS success: %s\n", bfs_success ? "yes" : "no");

  Position current_tile;
  std::array<Position, 4> neighbors;  // for debugging by pressing C during exec
                                      // with `show_neighbors = true`

  // --------------ENEMY--------------------------------------
  SDL_Texture* basic_mob_texture = load_mob_texture(renderer);

  const Dimension mob_shape = tileshape;
  Position mob_position = checkpoint1;

  Monster monster(mob_position, mob_shape, basic_mob_texture);

  // --------------CURSOR--------------------------------------
  SDL_Texture* cursor_texture = load_cursor_texture(renderer);

  Position cursor_tl = {0, 0};
  Position cursor = pixel_pos_from_grid(cursor_tl, tileshape);

  const int cursor_size = 2;
  const Dimension cursor_shape_tl = {cursor_size, cursor_size};
  const Dimension cursor_shape =
      pixel_shape_from_grid(cursor_shape_tl, tileshape);

  int fps = 0;

  // debug options
  bool show_buildable_tiles = false;
  bool show_traversable_tiles = false;
  bool show_monster = true;
  bool show_neighbors = false;

  // Game loop -----------------------------------------------------------------
  bool is_running = true;
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

        case SDL_MOUSEBUTTONDOWN: {
          if (event.button.button == SDL_BUTTON_LEFT) {
          }
          break;
        }

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_p:
              printf("FPS: %i\n", fps);
              break;
            case SDLK_t:
              printf("cursor gridwise position: ");
              cursor_tl.print();
              printf("cursor pixel position: ");
              cursor.print();
              printf("\n");
              break;

            case SDLK_c: {
              current_tile = cursor_tl;
              neighbors = map.neighboring_tiles(current_tile, &towers);
              break;
            }

            case SDLK_q: {
              if (can_put_tower_here(cursor_tl, &towers)) {
                Tower tower(cursor_tl, tower_size_tl, block_tower_texture);
                towers.push_back(tower);
              }
              break;
            }
            case SDLK_d: {
              delete_tower_at(cursor_tl, &towers);
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

    // render mob
    if (show_monster) {
      Position mob_pos_tl = grid_pos_from_pixel(monster.m_position, tileshape);
      distance_t curr_dist = distance_field.at(mob_pos_tl);

      if (curr_dist != 0) {
        Position next_mob_pos_tl;

        if (distance_field.min_neighbour(&towers, mob_pos_tl,
                                         &next_mob_pos_tl)) {
          Position next_mob_pos_px = tile_center(
              pixel_pos_from_grid(next_mob_pos_tl, tileshape), tileshape);
          monster.step_toward_simple(next_mob_pos_px);
          monster.render(renderer);
        }
      }
    }

    for (size_t i = 0; i < towers.size(); ++i) {
      (towers[i]).render(tileshape, renderer);
    }

    set_render_color(Color::BLACK, renderer);
    if (show_buildable_tiles) {
      for (Position tile_pos_tl : map.buildable_tiles) {
        highlight_tile(pixel_pos_from_grid(tile_pos_tl, tileshape), tileshape,
                       renderer);
      }
    }
    if (show_traversable_tiles) {
      for (Position tile_pos_tl : map.traversable_tiles) {
        highlight_tile(pixel_pos_from_grid(tile_pos_tl, tileshape), tileshape,
                       renderer);
      }
    }

    if (show_neighbors) {
      set_render_color(Color::BLUE, renderer);
      highlight_tile(pixel_pos_from_grid(current_tile, tileshape), tileshape,
                     renderer);
      set_render_color(Color::BLACK, renderer);
      for (size_t i = 0; i < neighbors.size(); i++) {
        highlight_tile(pixel_pos_from_grid(neighbors[i], tileshape), tileshape,
                       renderer);
      }
    }

    set_render_color(Color::BLACK, renderer);

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

void init_sdl(const Position screen_pos,
              const Dimension screen_shape,
              SDL_Window** return_window,
              SDL_Renderer** return_renderer) {
  *return_window =
      SDL_CreateWindow("Summermall TD", screen_pos.x, screen_pos.y,
                       screen_shape.w, screen_shape.h, SDL_WINDOW_OPENGL);

  if (*return_window == nullptr) {
    SDL_Log("Could not create a window: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  *return_renderer =
      SDL_CreateRenderer(*return_window, -1, SDL_RENDERER_ACCELERATED);

  if (*return_renderer == nullptr) {
    SDL_Log("Could not create a renderer: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
}

std::vector<Tower> setup_maze0(int tower_size_tl, SDL_Texture* tower_texture) {
  std::vector<Tower> towers;

  for (int i = 0; i < 8; ++i) {
    Position tower_pos1 = {4, 3};
    tower_pos1.x += i * tower_size_tl;
    Tower tower1(tower_pos1, tower_size_tl, tower_texture);
    towers.push_back(tower1);
  }
  for (int i = 0; i < 8; ++i) {
    Position tower_pos2 = {5, 6};
    tower_pos2.x += i * tower_size_tl;
    Tower tower2(tower_pos2, tower_size_tl, tower_texture);
    towers.push_back(tower2);
  }
  for (int i = 0; i < 8; ++i) {
    Position tower_pos3 = {4, 9};
    tower_pos3.x += i * tower_size_tl;
    Tower tower3(tower_pos3, tower_size_tl, tower_texture);
    towers.push_back(tower3);
  }
  return towers;
}

void get_hardcoded_graph_and_path_maze0(const Map* map,
                                        Dimension tileshape,
                                        WaypointGraph* return_graph,
                                        std::vector<Position>* return_path) {
  Position checkpoint1 = tile_center(
      pixel_pos_from_grid(map->checkpoint_tiles[0], tileshape), tileshape);
  Position checkpoint2 = tile_center(
      pixel_pos_from_grid(map->checkpoint_tiles[1], tileshape), tileshape);

  Position corner1 = pixel_pos_from_grid({20, 3}, tileshape);
  Position corner2 = pixel_pos_from_grid({20, 5}, tileshape);
  Position corner3 = pixel_pos_from_grid({5, 6}, tileshape);
  Position corner4 = pixel_pos_from_grid({5, 8}, tileshape);
  Position corner5 = pixel_pos_from_grid({20, 9}, tileshape);
  Position corner6 = pixel_pos_from_grid({20, 11}, tileshape);

  (*return_path).push_back(checkpoint1);
  (*return_path).push_back(corner1);
  (*return_path).push_back(corner2);
  (*return_path).push_back(corner3);
  (*return_path).push_back(corner4);
  (*return_path).push_back(corner5);
  (*return_path).push_back(corner6);
  (*return_path).push_back(checkpoint2);

  (*return_graph).add_vertices(return_path);
  (*return_graph).add_edge(checkpoint1, corner1);
  (*return_graph).add_edge(corner1, corner2);
  (*return_graph).add_edge(corner2, corner3);
  (*return_graph).add_edge(corner3, corner4);
  (*return_graph).add_edge(corner4, corner5);
  (*return_graph).add_edge(corner5, corner6);
  (*return_graph).add_edge(checkpoint2, corner6);

  // bad waypoints to test path finder
  Position node1 = pixel_pos_from_grid({4, 1}, tileshape);
  (*return_graph).add_edge(node1, checkpoint1);
  (*return_graph).add_edge(node1, corner1);
  Position node2 = pixel_pos_from_grid({19, 0}, tileshape);
  (*return_graph).add_edge(node2, checkpoint1);
  (*return_graph).add_edge(node2, corner1);
  (*return_graph).add_edge(node2, node1);
  Position node3 = pixel_pos_from_grid({4, 3}, tileshape);
  (*return_graph).add_edge(node3, node1);
  (*return_graph).add_edge(node3, corner1);
  Position node4 = pixel_pos_from_grid({21, 3}, tileshape);
  (*return_graph).add_edge(node4, checkpoint1);
  (*return_graph).add_edge(node4, corner1);
  (*return_graph).add_edge(node4, corner2);
  Position node5 = pixel_pos_from_grid({7, 5}, tileshape);
  (*return_graph).add_edge(node5, corner2);
  (*return_graph).add_edge(node5, corner3);
  Position node6 = pixel_pos_from_grid({15, 6}, tileshape);
  (*return_graph).add_edge(node6, corner2);
  (*return_graph).add_edge(node6, corner3);
  (*return_graph).add_edge(node6, node5);
  Position node7 = pixel_pos_from_grid({9, 9}, tileshape);
  (*return_graph).add_edge(node7, corner4);
  (*return_graph).add_edge(node7, corner5);
  Position node8 = pixel_pos_from_grid({5, 14}, tileshape);
  (*return_graph).add_edge(node8, corner6);
  (*return_graph).add_edge(node8, checkpoint2);
  Position node9 = pixel_pos_from_grid({13, 11}, tileshape);
  (*return_graph).add_edge(node9, corner6);
  (*return_graph).add_edge(node9, checkpoint2);
  (*return_graph).add_edge(node9, node8);
}

std::vector<Tower> setup_maze1(int tower_size_tl, SDL_Texture* tower_texture) {
  std::vector<Tower> towers;
  Position to_add;
  Tower tower({0, 0}, tower_size_tl, tower_texture);
  to_add = {4, 3};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {6, 3};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {8, 3};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {10, 3};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {12, 3};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {14, 3};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {16, 3};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {18, 3};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {18, 5};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {18, 7};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {18, 9};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {18, 11};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {18, 13};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {18, 15};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {4, 6};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {4, 8};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {4, 10};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {4, 12};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {4, 14};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {4, 16};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {4, 18};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {6, 18};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {8, 18};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {10, 18};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {7, 6};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {7, 8};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {9, 8};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {9, 10};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {11, 10};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {11, 12};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {13, 12};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {13, 14};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {15, 14};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {15, 16};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {15, 18};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {17, 18};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);
  to_add = {19, 18};
  tower = Tower(to_add, tower_size_tl, tower_texture);
  towers.push_back(tower);

  return towers;
}

SDL_Texture* load_mob_texture(SDL_Renderer* renderer) {
  const char* basic_mob_png_path =
      "assets/tower-defense-top-down/PNG/Default size/towerDefense_tile245.png";
  return SDL_CreateTextureFromSurface(renderer, IMG_Load(basic_mob_png_path));
}

SDL_Texture* load_block_tower_texture(SDL_Renderer* renderer) {
  const char* block_tower_png_path =
      "assets/tower-defense-top-down/PNG/Default size/towerDefense_tile180.png";
  return SDL_CreateTextureFromSurface(renderer, IMG_Load(block_tower_png_path));
}

SDL_Texture* load_tilesheet_texture(SDL_Renderer* renderer) {
  const char* tilesheet_png_path =
      "assets/tower-defense-top-down/Tilesheet/towerDefense_tilesheet.png";
  return SDL_CreateTextureFromSurface(renderer, IMG_Load(tilesheet_png_path));
}

SDL_Texture* load_cursor_texture(SDL_Renderer* renderer) {
  const char* cursor_png_path =
      "assets/tower-defense-top-down/PNG/Default size/towerDefense_tile015.png";
  return SDL_CreateTextureFromSurface(renderer, IMG_Load(cursor_png_path));
}

Map load_basic_map(void) {
  const char* basic_1P_map_tmx_path = "assets/maps/basic_1P.tmx";
  return parse_map_from_tmx(basic_1P_map_tmx_path);
}
