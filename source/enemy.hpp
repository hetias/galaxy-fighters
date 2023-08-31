#pragma once
#include<iostream>
#include<list>
#include<vector>

#include <SDL2/SDL.h>

#include"definitions.hpp"
#include"projectile.hpp"

//ENEMY DEFINITION
typedef struct enemy_s{
  SDL_FPoint position;
  SDL_FRect hitbox;
  SDL_Texture* sprite;
  SDL_Texture* projectile_texture;
  int hp;
  int shootDelay;
  int currentDelay;
}enemy_t;



//
//FUNCTION DEFINITIONS
//



enemy_t* enemy_create(std::vector<SDL_Texture*>*);
void enemy_update(enemy_t*, std::list<projectile_t*>*);
void enemy_draw(enemy_t*, SDL_Renderer*);
void enemy_destroy(enemy_t*);
