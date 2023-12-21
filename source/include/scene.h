#ifndef SCENE_H
#define SCENE_H

#include<stdio.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"definitions.h"
#include"ui.h"
#include"player.h"
#include"enemy.h"
#include"container.h"
#include"keyframe.h"
#include"spline.h"

#define MAX_KEYFRAMES 10
#define MAX_SPLINES   10

typedef struct{
    //entities and containers
    player_t* player;
    game_container projectiles_container;
    game_container enemies_container;
    int max_enemy_id;

    //resources
    SDL_Texture* textures_vector[TXT_TOTAL];

    //scene and level data
    int tick;
    
    int current_keyframe;
    int keyframe_count;
    keyframe_t keyframes[MAX_KEYFRAMES];
    spline_t splines[MAX_SPLINES];
    int spline_count;
}scene_t;

//scene management
scene_t* scene_create(const char**, SDL_Renderer*);
void scene_load_resources(scene_t*, const char**, SDL_Renderer*);

//scene functionalities
int scene_update(scene_t*);
int scene_draw(scene_t*, SDL_Renderer*);

int scene_update_projectiles(game_container*);
int scene_update_enemies(game_container*, game_container*);

int scene_draw_projectiles(game_container*, SDL_Renderer*);
int scene_draw_enemies(game_container*, SDL_Renderer*);

int scene_destroy(scene_t*);

//scene actions
bool scene_load_level(const char*, scene_t*);
void scene_next_action(scene_t*);
void scene_add_keyframe(scene_t*, keyframe_t);

#endif
