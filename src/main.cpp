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
#define MONSTER_GRID_SIZE 5  // pixels

// Regulate the fps to not exceed `FPS_TARGET_FPS` and return its estimated
// value.
//
// @param tick_start The tick given by `SDL_GetTicks()` at the start of game
// loop.
// @return The current regulated FPS estimate.
int fps_regulate_fps(Uint32 tick_start);

class Monster {
 public:
  Position m_position;
  float m_orientation;

 private:
  SDL_Texture* m_texture;
  Rectangle m_shape;

 public:
  Monster(Position position, float orientation) {
    m_position = position;
    m_orientation = orientation;
  }

  void set_texture(const Rectangle dst_shape,
                   const SDL_Rect src_tile_loc,
                   SDL_Texture* tilesheet,
                   SDL_Renderer* renderer) {
    m_shape = dst_shape;
    m_texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, m_shape.w, m_shape.h);
    assert(m_texture != NULL && "texture cant be NULL");

    SDL_SetRenderTarget(renderer, m_texture);
    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, tilesheet, &src_tile_loc, NULL);
    SDL_SetRenderTarget(renderer, NULL);
  }
  void render(SDL_Renderer* renderer) {
    SDL_Rect dst_tile_loc = {m_position.x, m_position.y, m_shape.w, m_shape.h};
    SDL_RenderCopy(renderer, m_texture, NULL, &dst_tile_loc);
  }
};

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
  const char* basic_1P_obstacles_tmx_path =
      "assets/maps/basic_1P_obstacles.tmx";
  const Map basic_1P_obstacles_map =
      parse_map_from_tmx(basic_1P_obstacles_tmx_path);

  // Load tilesheet
  const char* tilesheet_path =
      "assets/tower-defense-top-down/Tilesheet/towerDefense_tilesheet.png";
  SDL_Texture* tilesheet =
      SDL_CreateTextureFromSurface(renderer, IMG_Load(tilesheet_path));

  // Set size of tiles on screen
  const Rectangle tileshape = {
      basic_1P_obstacles_map.src_tileshape.w / ZOOM_DIVIDOR,
      basic_1P_obstacles_map.src_tileshape.h / ZOOM_DIVIDOR};

  // Create map texture

  SDL_Texture* static_map_texture = make_static_map_texture(
      &basic_1P_obstacles_map, tilesheet, tileshape, renderer);

  // a mob
  Position mob_position = {6 * 32, 0};
  float mob_orientation = 420.69;
  Monster monster(mob_position, mob_orientation);

  printf("%i\n", monster.m_position.x);
  printf("%i\n", monster.m_position.y);
  printf("%f\n", monster.m_orientation);

  // hardcoded location of mob in tilesheet
  Rectangle mob_src_shape = {64, 64};
  Position mob_src_position = {15, 10};
  SDL_Rect mob_in_tilesheet = {mob_src_position.x * mob_src_shape.w,
                               mob_src_position.y * mob_src_shape.h,
                               mob_src_shape.w, mob_src_shape.h};

  Rectangle mob_dst_shape = {mob_src_shape.w / ZOOM_DIVIDOR,
                             mob_src_shape.h / ZOOM_DIVIDOR};
  monster.set_texture(mob_dst_shape, mob_in_tilesheet, tilesheet, renderer);

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

    // Set the color to transparent
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Render map
    SDL_RenderCopy(renderer, static_map_texture, NULL, NULL);

    // render mob
    monster.render(renderer);

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
