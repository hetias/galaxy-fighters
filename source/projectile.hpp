#pragma once
#include<iostream>
#include<vector>
#include<list>

#include<SDL2/SDL.h>

#include"definitions.hpp"

#define FAST_PRJ_SPEED 30
#define NORMAL_PRJ_SPEED 15
#define SLOW_PRJ_SPEED 5

typedef struct projectile_s{
  SDL_FPoint position;
  SDL_FPoint direction;
  SDL_FRect hitbox;
  SDL_Texture* sprite;
  float speed;
  bool isFriendly;
}projectile_t;

typedef struct{
  projectile_t** array;
  size_t capacity;
  int count;
}projectiles_list;

//projectile function definitions
projectile_t* projectile_create(SDL_FPoint, SDL_FPoint, bool, SDL_Texture*);
int projectile_update(projectile_t*);
void projectile_draw(projectile_t*, SDL_Renderer*);
void projectile_destroy(projectile_t**);

//projectile list functions
projectiles_list projectiles_list_create();
int projectiles_list_add(projectiles_list*, projectile_t*);
int projectiles_list_remove(projectiles_list*, int);
int projectiles_list_clear(projectiles_list*);
int projectiles_list_rearrenge(projectiles_list*);
int projectiles_list_destroy(projectiles_list*);
int projectiles_list_is_empty(projectiles_list);
int projectiles_list_print(projectiles_list*);
int projectiles_list_no_capacity(projectiles_list);
