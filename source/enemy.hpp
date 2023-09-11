#pragma once
#include<iostream>

#include <SDL2/SDL.h>

#include"definitions.hpp"
#include"spline.hpp"
#include"projectile.hpp"

//ENEMY DEFINITION
typedef struct enemy_s{
  SDL_FPoint position;
  SDL_FRect hitbox;
  SDL_Texture* sprite;
  SDL_Texture* projectile_texture;
  int hp;
  int shootDelay;
  int currentDelay;
  spline* path;
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

enemy_t* enemy_create(std::vector<SDL_Texture*>);
void enemy_update(enemy_t*, std::list<projectile_t*>*);
void enemy_change_path(enemy_t*, spline*);
void enemy_draw(enemy_t*, SDL_Renderer*);
void enemy_destroy(enemy_t*);
void enemy_update_path(enemy_t*);

enemies_container* enemies_container_create();
int enemies_container_add(enemies_container*, enemy_t*);
int enemies_container_remove(enemies_container*, int);
int enemies_container_clear(enemies_container*);
int enemies_container_rearrenge(enemies_container*);
int enemies_container_destroy(enemies_container**);
bool enemies_container_is_empty(enemies_container*);
void enemies_container_print(enemies_container*);
