#ifndef RESOURCES_H
#define RESOURCES_H

#include<SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "definitions.h"

//forward declarations
typedef struct{
    SDL_Texture *textures[TXT_TOTAL];
    TTF_Font    *font;
}resources_t;

SDL_Texture *get_rs_texture(const char*);

#endif
