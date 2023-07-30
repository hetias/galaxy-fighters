#pragma once
#include<iostream>
#include<vector>
#include<list>

#include<SDL2/SDL.h>

#include"definitions.hpp"

typedef struct projectile_s{
  SDL_FPoint position;
  SDL_FPoint direction;
  SDL_FRect hitbox;
  SDL_Texture* sprite;
  float speed;
  bool isFriendly;
}projectile_t;

//global game textures vector
//found in main.cpp
extern std::vector<SDL_Texture*> gTextures;

//global SDL_Renderer object
//found in main.cpp
extern SDL_Renderer* gRenderer;

//global projectiles vector.
//found in main.cpp
extern std::list<projectile_t*> gProjectiles;

projectile_t* projectile_create(SDL_FPoint, SDL_FPoint);
void projectile_update(projectile_t*);
void projectile_draw(projectile_t*);
void projectile_destroy(projectile_t*);
