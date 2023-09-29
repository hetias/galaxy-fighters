#pragma once
#include<stdio.h>

#include <SDL2/SDL.h>

#include"definitions.h"
//#include"spline.hpp"
//#include"projectile.hpp"

//ENEMY DEFINITION
typedef struct enemy_s{
  SDL_FPoint position;
  SDL_FRect hitbox;
  SDL_Texture* sprite;
  SDL_Texture* projectile_texture;
  int hp;
  int shootDelay;
  int currentDelay;
  //spline* path;
  float path_time;
}enemy_t;

typedef struct{
  enemy_t** array;
  int capacity;
  int count;
}enemies_container;

//
//FUNCTION DEFINITIONS
//
enemy_t* enemy_create(SDL_Texture**);
void enemy_update(enemy_t*);
//void enemy_change_path(enemy_t*, spline*);
void enemy_draw(enemy_t*, SDL_Renderer*);
void enemy_destroy(enemy_t*);
void enemy_update_path(enemy_t*);

