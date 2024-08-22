#ifndef EDITOR_H
#define EDITOR_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"core/definitions.h"

#include"core/resources.h"
#include"core/scene.h"
#include "core/gg.h"
#include "core/timing.h"

bool editor_start(resources_t* game_resources, SDL_Window* window);

void editor_events(SDL_Event e);
void editor_running();
void editor_draw(SDL_Renderer* r);
void editor_exit();

static bool is_hovering(SDL_Point, SDL_Rect);
static int get_spline_count();
static void add_spline(spline_t);
static void remove_spline(int);
static spline_t* get_splines();

static bool ui_button(SDL_Rect rect, const char*, SDL_Renderer*);

#endif
