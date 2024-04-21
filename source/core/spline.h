#ifndef SPLINE_H
#define SPLINE_H

#include<stdio.h>
#include<stdbool.h>

#include"SDL2/SDL.h"
#include"definitions.h"

#define MAX_POINTS 12

typedef struct{
  size_t total_points;
  SDL_FPoint points[MAX_POINTS];
  bool loop;
}spline_t;

spline_t spline_create(bool);
bool spline_add_point(spline_t*, SDL_FPoint);
bool spline_add_points(spline_t*, SDL_FPoint*, int);
void spline_clean(spline_t*);
SDL_FPoint spline_get_point(spline_t, float);
SDL_FPoint to_worldCoords(SDL_FPoint);
SDL_FPoint to_fixedCoords(SDL_FPoint);

void spline_draw(spline_t, SDL_Renderer *);

// this functions expects splines points to be
// on (0, 1) range
void spline_draw_fixed(spline_t, SDL_Point, SDL_Renderer*);
void spline_draw_world(spline_t, SDL_Point, SDL_Renderer*);

#endif
