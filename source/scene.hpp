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
  projectiles_list projectiles;
  //std::list<projectile_t*>projectile_list;
  std::vector<SDL_Texture*> textures_vector;
}scene_t;

scene_t* scene_create(const char**);
void scene_load_resources(scene_t*, const char**);

int scene_update(scene_t*);
int scene_draw(scene_t*, SDL_Renderer*);

int scene_update_projectiles(projectiles_list*);
int scene_update_enemies(enemies_container**);

int scene_draw_projectiles(projectiles_list, SDL_Renderer*);
int scene_draw_enemies(enemies_container*, SDL_Renderer*);

int scene_destroy(scene_t*);
