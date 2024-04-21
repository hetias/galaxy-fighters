#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"core/definitions.h"

#include"core/resources.h"
#include"core/scene.h"
#include"core/gg.h"
#include "core/spline.h"

enum ObjectType{
    OBJECT_ENEMY,
    OBJECT_SPLINE,
};

typedef struct sObject{
    int type;  
    SDL_Point position;
    void *obj;
}Object;

int init(void);
void deinit(void);
void load_resources(void);
void free_resources(void);
void loop(void);

SDL_Point word_to_screen(SDL_Point);
SDL_Point screen_to_word(SDL_Point);

SDL_Renderer *gRenderer = NULL;
SDL_Window     *gWindow = NULL;
bool    gIsGameRunning = true;
bool           gPaused = false;

resources_t game_resources;

SDL_Point screen_res = {WINDOW_WIDTH, WINDOW_HEIGHT};

typedef struct spline_s{
    int top;
    spline_t arr[64];
}spline_stack;

spline_stack splines = {-1, {0}};

//word info
SDL_Rect  game_space = {0, 0, 0, 0}; //
SDL_Point gWord_center = {0, 0};

SDL_Rect player_rect = {0, 0, 0, 0};

const char* gTexturesPaths[] ={
    //background
    "resources/sprites/backgrounds/black.png",
    "resources/sprites/backgrounds/purple.png",
    "resources/sprites/backgrounds/blue.png",
    "resources/sprites/backgrounds/darkPurple.png",
    //Player
    "resources/sprites/player/player_blue.png",
    //Enemy
    "resources/sprites/enemies/enemyBlack1.png",
    //Lasers
    "resources/sprites/lasers/laserBlue.png",
    "resources/sprites/lasers/laserRed.png",
    //ui
    "resources/sprites/ui/lifeblue1.png"
};

TTF_Font *gFont = NULL;

int main(){
    if(init() < 0){
	SDL_Log("failed on initialization\n");
	deinit();
    }

    load_resources();
    
    loop();

    free_resources();

    deinit();
    return 0;
}

void
loop(void){
    //figure out the size of the player texture
    SDL_QueryTexture(game_resources.textures[TXT_PLAYER_BLUE], NULL, NULL, &player_rect.w, &player_rect.h);

    //get the center of the screen and the game space
    gWord_center = (SDL_Point){screen_res.x / 2, screen_res.y / 2};
    game_space = (SDL_Rect){0, 0, screen_res.x, screen_res.x}; 

    //center the player's texture
    player_rect.x = gWord_center.x - player_rect.w / 2;
    player_rect.y = gWord_center.y - player_rect.h / 2;    

    //gg_init(gRenderer, game_resources.font);
    
    //events();
    SDL_Event e;
    while(gIsGameRunning){

	while(SDL_PollEvent(&e)){
	    //gg_events(e);
	    
	    switch(e.type){
	    case SDL_QUIT:
		gIsGameRunning = false;
		break;
	    case SDL_WINDOWEVENT:
		//handling window events
		switch(e.window.event){
		case SDL_WINDOWEVENT_RESIZED:
                    #if DEBUG
		    SDL_Log("Window %d resized to %d x %d\n", e.window.windowID, e.window.data1, e.window.data2);
                    #endif
		    
		    screen_res.x = e.window.data1;
		    screen_res.y = e.window.data2;
		    
		    gWord_center.x = screen_res.x / 2;
		    gWord_center.y = screen_res.y / 2;

		    player_rect.x = gWord_center.x - (player_rect.w / 2);
		    player_rect.y = gWord_center.y - (player_rect.h / 2);
		    
		    game_space.w = screen_res.y;
		    game_space.h = screen_res.y;

		    game_space.x = gWord_center.x - (game_space.w / 2);
		    game_space.y = 0;
		    
		    break;
		}
		break;
	    }
	}
	//logic();	
	const int rsize = 20;

	SDL_Rect mp = {0, 0};
	SDL_GetMouseState(&mp.x, &mp.y);

	const uint8_t* keys = SDL_GetKeyboardState(NULL);

	if(keys[SDL_SCANCODE_N] ){
	    printf("new spline");

	    splines.top += 1;
	    spline_t s;
	    
	    s.loop = false;
	    s.points[0] = (SDL_FPoint){100.0f, 100.0f};
	    s.points[1] = (SDL_FPoint){200.0f, 200.0f};
	    s.points[2] = (SDL_FPoint){300.0f, 200.0f};
	    s.points[3] = (SDL_FPoint){400.0f, 100.0f};
	    s.total_points = 4;
	    splines.arr[splines.top] = s;
	}
	
	//draw();
	SDL_SetRenderDrawColor(gRenderer, 100, 25, 75, 255);
	SDL_RenderClear(gRenderer);
	
	//bg
	SDL_RenderCopy(gRenderer, game_resources.textures[TXT_BG_PURPLE], NULL, &game_space);		
	
	//render player
	SDL_RenderCopy(gRenderer, game_resources.textures[TXT_PLAYER_BLUE], NULL, &player_rect);

	#define SR_SIZE 25
	SDL_FPoint p = {0};
	SDL_Rect sr = {0};
	for(int i = 0; i < splines.top; i++){
	    for(int j = 0; j < splines.arr[i].total_points; j++){
		p = splines.arr[i].points[j];
		sr = (SDL_Rect){(p.x - (float)SR_SIZE / 2 ) + game_space.x,
				(p.y - (float)SR_SIZE / 2) + game_space.y,
				SR_SIZE, SR_SIZE};	   
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(gRenderer, &sr);
	    }

            spline_draw_world(splines.arr[i], (SDL_Point){game_space.x, game_space.y}, gRenderer);	    
	}
	SDL_RenderPresent(gRenderer);
    }
}

int
init(void){
    int success = 0;

    success = SDL_Init(SDL_INIT_EVERYTHING);
    if(success != 0){
	SDL_Log("Failed initializing sdl2\n");
	return -1;
    }

    success = IMG_Init(IMG_INIT_PNG);
    if(success != IMG_INIT_PNG){
	SDL_Log("Failed initializing SDL_IMG\n");
	return -1;
    }
    
    success = TTF_Init();
    if(success < 0){
	SDL_Log("Failed initializ SDL_TTF\n");
	return -1;
    }    

    
    gWindow = SDL_CreateWindow("window",
			      SDL_WINDOWPOS_CENTERED,
			      SDL_WINDOWPOS_CENTERED,
			      screen_res.x,
			      screen_res.y,
			      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(gWindow == NULL){
	SDL_Log("Failed on window creation\n");
	return -1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    if(gRenderer == NULL){
	SDL_Log("Failed on renderer creation\n");
	return -1;
    }

    return success;
}

void
load_resources(void){
    for(int i = 0; i < TXT_TOTAL; i++){
	game_resources.textures[i] = NULL;
	game_resources.textures[i] =
	    IMG_LoadTexture(gRenderer, gTexturesPaths[i]);
	if(game_resources.textures[i] == NULL){
	    SDL_Log("Warning: failed on resouces load: %s\n",
		   gTexturesPaths[i]);
	}
    }

    const char* fontPath = "resources/fonts/font.ttf";
    game_resources.font = TTF_OpenFont(fontPath,16);
    if(game_resources.font == NULL){
	SDL_Log("Warning: failed on resource load: %s\n", fontPath);
    }
}

void
free_resources(void){
    for(int i = 0; i < TXT_TOTAL; i++){
	SDL_DestroyTexture(game_resources.textures[i]);
	game_resources.textures[i] = NULL;
    }

    TTF_CloseFont(game_resources.font);
}

void
deinit(void){
    if(gRenderer){
	SDL_DestroyRenderer(gRenderer);	
    }

    if(gWindow){
	SDL_DestroyWindow(gWindow);
    }

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

SDL_Point word_to_screen(SDL_Point pPos){
    SDL_Point p = {pPos.x + gWord_center.x,
		   pPos.y + gWord_center.y};
    return p;
}

SDL_Point screen_to_word(SDL_Point pPos){
    SDL_Point p = {pPos.x - gWord_center.x,
		   pPos.y - gWord_center.y };
    return p;
}
