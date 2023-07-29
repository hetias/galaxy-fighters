#pragma once

#include<iostream>
#include<vector>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include"projectile.hpp"

//global game textures vector
//found in main.cpp
extern std::vector<SDL_Texture*> gTextures;

//global SDL_Renderer object
//found in main.cpp
extern SDL_Renderer* gRenderer;

//global texture name references
//extern int player_texture;

//global projectiles container
extern std::vector<projectile_t*> gProjectiles;

//player definition
typedef struct player_s{
  SDL_FPoint position;
  SDL_FPoint direction;
  SDL_FRect hitbox;
  int shootDelay;
  int currentShootDelay;
  int hp;
  float speed;
  SDL_Texture* sprite;
}player_t;

//creation of player entity.
//argument it's the texture to use
//returns a player ready to use.
player_t* player_create();

//player update.
//arguments are a player_t* and a reference to SDL_KeyboardState
void player_update(player_t*, const Uint8*);

//player draw
//Draw ship in player position
//But centered
void player_draw(player_t*);

//Player_destroy
//free player memory
void player_destroy(player_t*);
