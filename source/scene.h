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
#include"spline.h"

typedef struct{
    player_t* player;
    game_container projectiles_container;
    game_container enemies_container;
    int tick;  
    SDL_Texture* textures_vector[TXT_TOTAL];
    spline_t splines;
}scene_t;

scene_t* scene_create(const char**, SDL_Renderer*);
void scene_load_resources(scene_t*, const char**, SDL_Renderer*);

int scene_update(scene_t*);
int scene_draw(scene_t*, SDL_Renderer*);

int scene_update_projectiles(game_container*);
int scene_update_enemies(game_container*, game_container*);

int scene_draw_projectiles(game_container*, SDL_Renderer*);
int scene_draw_enemies(game_container*, SDL_Renderer*);

int scene_destroy(scene_t*);

#endif
