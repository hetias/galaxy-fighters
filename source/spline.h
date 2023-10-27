#ifndef SPLINE_H
#define SPLINE_H

#include<stdio.h>
#include<stdbool.h>

#include"SDL2/SDL.h"

#define MAX_POINTS 12

typedef struct{
  size_t total_points;
  SDL_FPoint points[MAX_POINTS];
  bool loop;
}spline_t;

spline_t spline_create(bool);
void spline_add_point(spline_t*, SDL_FPoint);
void spline_add_points(spline_t*, SDL_FPoint*);
void spline_clean(spline_t*);
SDL_FPoint spline_get_point(spline_t, float);
void spline_draw(spline_t, SDL_Renderer*);

#endif
