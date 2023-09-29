#ifndef PROJECTILES_H
#define PROJECTILES_H

#include<stdio.h>
#include<stdbool.h>
#include<SDL2/SDL.h>

#include"definitions.h"

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

//projectile function definitions
projectile_t* projectile_create(SDL_FPoint, SDL_FPoint, bool, SDL_Texture*);
int projectile_update(projectile_t*);
void projectile_draw(projectile_t*, SDL_Renderer*);
void projectile_destroy(projectile_t**);

#endif
