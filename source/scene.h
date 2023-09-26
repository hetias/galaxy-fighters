#include<stdio.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"definitions.h"
//#include"player.h"
//#include"enemy.hpp"
//#include"projectile.hpp"
//#include"spline.h"

typedef struct{
  //player_t* main_player;
  //enemies_container* enemies_array;
  //projectiles_list projectiles;
  int tick;  
  SDL_Texture* textures_vector[TXT_TOTAL];
}scene_t;

scene_t* scene_create(const char**, SDL_Renderer*);
void scene_load_resources(scene_t*, const char**, SDL_Renderer*);

int scene_update(scene_t*);
int scene_draw(scene_t*, SDL_Renderer*);

//int scene_update_projectiles(projectiles_list*);
//int scene_update_enemies(enemies_container**);

//int scene_draw_projectiles(projectiles_list, SDL_Renderer*);
//int scene_draw_enemies(enemies_container*, SDL_Renderer*);

int scene_destroy(scene_t*);
