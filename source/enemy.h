#ifndef ENEMY_H
#define ENEMY_H

#include<stdio.h>

#include <SDL2/SDL.h>

#include "core/definitions.h"
#include "core/spline.h"
#include "core/container.h"
#include "projectile.h"
#include "core/timing.h"

typedef enum EPATH_STATE{
    PATH_STATE_FORWARD,
    PATH_STATE_BACKWARD,
    PATH_STATE_START,
    PATH_STATE_END
}PATH_STATE;

typedef enum EPATH_TYPE{
    PATH_START_END,
    PATH_REPEAT,
    PATH_LOOP
}PATH_TYPE;

typedef enum{
    ENEMY_TYPE_NORMAL,
    ENEMY_TYPE_TRIPLE,
    ENEMY_TYPE_HOMING,
    ENEMY_TYPE_LASER,
    ENEMY_TYPE_SHIELD,
}ENEMY_TYPE;

//ENEMY DEFINITION
typedef struct enemy_s{
    SDL_FPoint position;
    SDL_FRect hitbox;
    SDL_Texture* sprite;
    SDL_Texture* projectile_texture;
    int id;
    int hp;
    int shootDelay;
    int currentDelay;
    float speed;
    spline_t* path;
    float path_time;
    PATH_STATE path_state;
    PATH_TYPE path_type;
}enemy_t;

extern time_info gametime;

//
//FUNCTION DEFINITIONS
//
enemy_t enemy_create(SDL_Texture**, int);
int enemy_update(enemy_t*, array_list*);
void enemy_change_path(enemy_t*, spline_t*);
void enemy_draw(enemy_t*, SDL_Renderer*);
static PATH_STATE enemy_update_path(enemy_t*);
static void enemy_path_ended(enemy_t*);

void enemy_set_can_shoot(enemy_t*, bool, int);

#endif
