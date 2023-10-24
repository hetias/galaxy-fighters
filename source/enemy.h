#ifndef ENEMY_H
#define ENEMY_H

#include<stdio.h>

#include <SDL2/SDL.h>

#include"definitions.h"
//#include"spline.hpp"
#include"container.h"
#include"projectile.h"

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

//
//FUNCTION DEFINITIONS
//
enemy_t* enemy_create(SDL_Texture**);
int enemy_update(enemy_t*, game_container*);
//void enemy_change_path(enemy_t*, spline*);
void enemy_draw(enemy_t*, SDL_Renderer*);
void enemy_destroy(enemy_t**);
void enemy_update_path(enemy_t*);
void enemy_path_ended(enemy_t*);

void enemy_set_can_shoot(enemy_t*, bool, int);

#endif
