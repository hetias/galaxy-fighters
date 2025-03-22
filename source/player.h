#ifndef PLAYER_H
#define PLAYER_H

#include<math.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include"core/definitions.h"
#include"core/timing.h"

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

#include"core/container.h"
#include"projectile.h"

extern time_info gametime;

//
//FUNCTION DEFINITIONS
//

//creation of player entity.
//argument it's the texture to use
//returns a player ready to use.
player_t* player_create(SDL_Texture*[]);

//player update.
//arguments are a player_t* and a reference to SDL_KeyboardState
int player_update(player_t*,  const Uint8*, array_list*);

//player draw
//Draw ship in player position
//But centered
void player_draw(player_t*,  SDL_Renderer*);

//Player_destroy
//free player memory
void player_destroy(player_t*);

#endif
