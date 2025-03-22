#ifndef PROJECTILES_H
#define PROJECTILES_H

#include<stdio.h>
#include<stdbool.h>
#include<SDL2/SDL.h>

#include"core/definitions.h"
#include"core/container.h"
#include"enemy.h"
#include"player.h"
#include"core/timing.h"

#define FAST_PRJ_SPEED 750.0f
#define NORMAL_PRJ_SPEED 500.0f
#define SLOW_PRJ_SPEED 250.0f

typedef struct projectile_s{
    SDL_FPoint position;
    SDL_FPoint direction;
    SDL_FRect hitbox;
    SDL_Texture* sprite;
    float speed;
    bool isFriendly;
    bool alive;
}projectile_t;

extern time_info gametime;

//projectile function definitions
projectile_t projectile_create(SDL_FPoint, SDL_FPoint, bool, SDL_Texture*);
int projectile_update(projectile_t*, array_list*, player_t*);
void projectile_draw(projectile_t*, SDL_Renderer*);
#endif
