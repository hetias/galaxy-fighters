#include<iostream>

#include"definitions.hpp"
#include"player.hpp"
#include"enemy.hpp"
#include"projectile.hpp"
#include"spline.hpp"

typedef struct{
  Uint32 tick;
  player_t* main_player;
  //std::vector<enemy_t*>enemy_vector;
  enemies_container* enemies_array;
  std::list<projectile_t*>projectile_list;
  std::vector<SDL_Texture*> textures_vector;
}scene_t;

scene_t* scene_create(const char**);
void scene_load_resources(scene_t*, const char**);

void scene_update(scene_t*);
void scene_draw(scene_t*, SDL_Renderer*);

void scene_update_projectiles(std::list<projectile_t*>*);
int scene_update_enemies(enemies_container**);

void scene_draw_projectiles(scene_t*, SDL_Renderer*);
int scene_draw_enemies(enemies_container*, SDL_Renderer*);

void scene_destroy(scene_t*);
