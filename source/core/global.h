#ifndef GLOBAL_H
#define GLOBAL_H

#include<stdbool.h>

#include"SDL2/SDL_video.h"
#include"SDL2/SDL_render.h"

#include"scene.h"
#include"timing.h"
#include"resources.h"

typedef struct global_state_s{
    SDL_Renderer *renderer;
    SDL_Window *window;
    scene_t *scene;
    time_info time;
    debug_info_t debug_info;
    resources_t resources;
    bool is_running;
    bool is_paused;
    int app_state;
    struct nk_context *gui_context;
}global_state;

#endif



