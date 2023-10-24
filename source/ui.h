#ifndef UI_H
#define UI_H

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"SDL2/SDL_ttf.h"
#include"definitions.h"
#include"player.h"

#define HP_W 33
#define HP_H 26
#define X_GAP 5
#define Y_GAP 15

typedef struct{
    Uint8 time;
    SDL_Texture* texture;
    SDL_Point position;
    SDL_Point clip;
}ui_hp_icon;

void draw_ui(player_t*, SDL_Texture**, SDL_Renderer*);
void draw_text(SDL_Renderer*);
SDL_Point to_ui_coords(SDL_Point);
SDL_Point to_screen_coords(SDL_Point);

#endif
