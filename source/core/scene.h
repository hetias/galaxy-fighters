#ifndef SCENE_H
#define SCENE_H

#include<stdio.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"

#include"definitions.h"

#include"container.h"
#include"resources.h"
#include"../player.h"
#include"../enemy.h"
#include"keyframe.h"
#include"spline.h"

#include"debug_draw.h"

#define MAX_KEYFRAMES 10
#define MAX_SPLINES   10

typedef struct{
    //entities and containers
    player_t* player;
    array_list projectiles_container;
    array_list enemies_container;
    int max_enemy_id;

    //resources
    resources_t *resources;

    //scene and level data
    int tick;

    //keyframes info
    int current_keyframe;
    int keyframe_count;
    keyframe_t keyframes[MAX_KEYFRAMES];

    //splines info
    spline_t splines[MAX_SPLINES];
    int spline_count;
}scene_t;

//scene management
scene_t* scene_create(resources_t*, const char*);

//scene functionalities
int scene_update(scene_t*);
int scene_draw(scene_t*, SDL_Renderer*);

int scene_update_projectiles(array_list*, array_list*, player_t*);
int scene_update_enemies(array_list*, array_list*);

int scene_draw_projectiles(array_list*, SDL_Renderer*);
int scene_draw_enemies(array_list*, SDL_Renderer*);

int scene_destroy(scene_t*);

//scene actions
bool scene_load_level(const char*, scene_t*);
void scene_next_action(scene_t*);
void scene_add_keyframe(scene_t*, keyframe_t);

#endif
