#include"gg.h"

static gg_state ui_state;

int gg_init(SDL_Renderer* renderer, TTF_Font *font){
    if(renderer == NULL || font == NULL)
	return -1;
    
    ui_state.maxid            = -1;
    ui_state.mousex	      = 0;
    ui_state.mousey	      = 0;
    ui_state.mousedown	      = 0;
    ui_state.hotitem	      = -1;
    ui_state.activeitem	      = -1;
    ui_state.renderer	      = renderer;
    ui_state.font             = font;

    return 0;
}

void gg_deinit(){
    ui_state.renderer = NULL;
}

void gg_events(SDL_Event event){
    switch(event.type){

	//MOUSE POSITION
    case SDL_MOUSEMOTION:
	ui_state.mousex = event.motion.x;
	ui_state.mousey = event.motion.y;
	break;

	//MOUSE CLICLK
    case SDL_MOUSEBUTTONDOWN:
	if(event.button.button == 1)
	    ui_state.mousedown = 1;	    
	break;

	//MOUSE STOP CLICK
    case SDL_MOUSEBUTTONUP:
	if(event.button.button == 1)
	    ui_state.mousedown = 0;
	break;
    }
}

// * GG ELEMENTS *

bool gg_button(int _id, SDL_Rect rect){
    ui_state.maxid +=1;
    int id = ui_state.maxid;
    
    bool clicked = false;
    SDL_Color color = {125, 125, 125, 125};

    //interact with button
    if(on_region(rect.x, rect.y, rect.w, rect.h)){
	ui_state.hotitem = id;
	color = (SDL_Color){0, 0, 255, 0};

	if(ui_state.mousedown && ui_state.activeitem == -1){
	    ui_state.activeitem = id;
	}
    }

    if(ui_state.activeitem == id &&
       ui_state.hotitem == id &&
       ui_state.mousedown == 0){
	clicked = true;
	color = (SDL_Color){255, 0, 0, 0};	
    }
    //draw the button
    draw_rect(rect, color);
    
    return clicked;
}

void gg_label(SDL_Rect rect, const char* text, int size){
    TTF_SetFontSize(ui_state.font, size);
    
    if(ui_state.font == NULL){
	printf("no font loaded\n");
	return;
    }

    SDL_Surface *text_surface = NULL;
    text_surface = TTF_RenderText_Solid(ui_state.font, text, (SDL_Color){255, 255, 255});
    if(text_surface == NULL){
	printf("Failed renderering text surface\n");
	
	return;
    }

    SDL_Texture *text_texture = NULL;
    text_texture = SDL_CreateTextureFromSurface(ui_state.renderer, text_surface);
    if(text_texture == NULL){
	printf("Failed creating text texture\n");
	return;
    }
    
    SDL_Rect final_rect = {rect.x, rect.y, text_surface->w, text_surface->h};
    SDL_RenderCopy(ui_state.renderer, text_texture, NULL, &final_rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

void gg_draw(){
    
}

void gg_begin(){
    ui_state.hotitem = -1;
}

void gg_end(){
    ui_state.maxid = -1;
    //if mouse is released theres no active items anymore
    //else 
    if(ui_state.mousedown == 0){
	ui_state.activeitem = -1;
    }
}

static void draw_rect(SDL_Rect rect, SDL_Color color){
    SDL_SetRenderDrawColor(ui_state.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(ui_state.renderer, &rect);
}

static int on_region(int x, int y, int w, int h){
    if(ui_state.mousex < x ||
       ui_state.mousey < y ||
       ui_state.mousex >= x + w ||
       ui_state.mousey >= y + h)
	return 0;
    return 1;
}
