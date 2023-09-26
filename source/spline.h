#pragma once
#include<stdio.h>
#include<stdbool.h>

#include"SDL2/SDL.h"

#define MAX_POINTS 12

typedef struct{
  size_t total_points;
  SDL_FPoint points[MAX_POINTS];
  bool loop;
}spline;

spline spline_create(bool);
void spline_add_point(spline*, point);
void spline_clean(spline*);
SDL_FPoint spline_get_point(spline, float);
void spline_draw(spline, SDL_Renderer*);
