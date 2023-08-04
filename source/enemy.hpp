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
  int hp;
}enemy_t;

//
//EXTERN VARIABLES
//



//global SDL_Renderer object
//found in main.cpp
extern SDL_Renderer* gRenderer;

//global game textures vector
//found in main.cpp
extern std::vector<SDL_Texture*> gTextures;

//global projectiles container
extern std::list<projectile_t*> gProjectiles;

//
//FUNCTION DEFINITIONS
//



enemy_t* enemy_create();
void enemy_update(enemy_t*);
void enemy_draw(enemy_t*);
void enemy_destroy(enemy_t*);
