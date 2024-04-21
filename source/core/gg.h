#ifndef GG_H
#define GG_H

#include<stdbool.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

enum Emenu_options{
    MENU_PLAY = 0,
    MENU_RESTART,
    MENU_OPTIONS,
    MENU_EXIT
};

typedef struct{
    int           maxid;
    int           mousex;
    int           mousey;
    int           mousedown;
    int           hotitem;
    int           activeitem;
    SDL_Renderer *renderer;
    TTF_Font     *font;
}gg_state;

int gg_init(SDL_Renderer*, TTF_Font*);
void gg_deinit();
void gg_begin();
void gg_end();
void gg_events(SDL_Event);
void gg_draw();
bool gg_button(int id, SDL_Rect rect);
void gg_label(SDL_Rect, const char*, int size);



static void draw_rect(SDL_Rect rect, SDL_Color color);
static int on_region(int x, int y, int w, int h);

#endif
