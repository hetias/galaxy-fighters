#ifndef UI__H
#define UI_H

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"SDL2/SDL_ttf.h"
#include"definitions.h"

typedef struct{
    SDL_Texture *texture;
    int          size;
    char        *text;
    SDL_Point    size;
    SDL_Point    clip;
}ui_element;

void draw_ui_element(ui_element, SDL_Renderer*);
void draw_text(SDL_Point, const char*, SDL_Renderer*);
SDL_Point to_ui_coords(SDL_Point);
SDL_Point to_screen_coords(SDL_Point);

#endif
