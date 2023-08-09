#pragma once
#include<iostream>
#include<vector>
#include<SDL2/SDL.h>

extern SDL_Renderer* gRenderer;

typedef struct{
  std::vector<SDL_FPoint> points;
  bool loop;
}spline;

SDL_FPoint spline_get_point(spline, float);
void spline_draw(spline);
