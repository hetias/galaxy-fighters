#ifndef EDITOR_H
#define EDITOR_H

#include<SDL2/SDL_render.h>
#include<SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>


#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

//Nuklear Immediate Mode GUI library
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#define NK_SDL_RENDERER_SDL_H "SDL2/SDL.h"
#include "nuklear.h"
#include "demo/sdl_renderer/nuklear_sdl_renderer.h"

#include "core/definitions.h"
#include "core/resources.h"
#include "core/scene.h"
#include "core/timing.h"

typedef struct {
    int spline_id;
    int point_id;
}spline_point_selected_t;

bool editor_start();

void editor_events(SDL_Event e);
void editor_running(struct nk_context*);
void editor_draw(SDL_Renderer* r);
void editor_exit();
static bool editor_save_to_file();

//static bool is_hovering(SDL_Point, SDL_Rect);
static int get_spline_count();
static void add_spline(spline_t);
static void add_default_spline();
static void remove_spline(int);
static spline_t *get_splines();

static keyframe_t *get_keyframes();
static int get_keyframe_count();
static void add_keyframe(keyframe_t);

static void draw_timeline(struct nk_context*);
static void spline_editor();
static bool ui_button(SDL_Rect rect, const char*, SDL_Renderer*);
static bool on_keyframe();

#endif
