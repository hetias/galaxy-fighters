#pragma once

#include<iostream>
#include<list>
#include<vector>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include"projectile.hpp"

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
  SDL_Texture* projectile_sprite;
}player_t;


//
//FUNCTION DEFINITIONS
//

//creation of player entity.
//argument it's the texture to use
//returns a player ready to use.
player_t* player_create(std::vector<SDL_Texture*>*);

//player update.
//arguments are a player_t* and a reference to SDL_KeyboardState
void player_update(player_t*,  const Uint8*, std::list<projectile_t*>*);

//player draw
//Draw ship in player position
//But centered
void player_draw(player_t*,  SDL_Renderer*);

//Player_destroy
//free player memory
void player_destroy(player_t*);
