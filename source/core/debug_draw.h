#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_render.h"

#include "resources.h"

enum draw_type{
  RECT,
  LINE,
  TEXT,
};

typedef struct debug_data_s{
  const char* text;
  int type;
  int x;
  int y;
  int x2;
  int y2;
  SDL_Color color;
}debug_data_t;

typedef struct debug_info_s{
  debug_data_t data[16];
  SDL_Texture *text_texture;
  SDL_Surface *text_surface;
  int top;
}debug_info_t;

extern SDL_Renderer *renderer;
extern debug_info_t g_debug_info;
extern resources_t game_resources;

void debug_start();
void debug_clean();

void debug_text(const char*, int, int, SDL_Color);
void debug_rect(int, int, int, int, SDL_Color);
void debug_line(int, int, int, int, SDL_Color);

static void debug_draw_rect(debug_data_t);
static void debug_draw_line(debug_data_t);
static void debug_draw_text(debug_data_t);

void debug_draw_present();

#endif 
