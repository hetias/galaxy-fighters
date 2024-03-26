#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"core/definitions.h"

#include"core/resources.h"
#include"core/scene.h"
#include"core/gg.h"

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

SDL_Point gWindow_size = {WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Point gWord_center = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

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
	printf("failed on initialization\n");
	deinit();
    }

    load_resources();

    loop();

    free_resources();

    deinit();
    return 0;
}

int
init(void){
    int success = 0;

    success = SDL_Init(SDL_INIT_EVERYTHING);
    if(success != 0){
	printf("Failed initializing sdl2\n");
	return -1;
    }

    success = IMG_Init(IMG_INIT_PNG);
    if(success != IMG_INIT_PNG){
	printf("Failed initializing SDL_IMG\n");
	return -1;
    }
    
    success = TTF_Init();
    if(success < 0){
	printf("Failed initializ SDL_TTF\n");
	return -1;
    }    

    
    gWindow = SDL_CreateWindow("window",
			      SDL_WINDOWPOS_CENTERED,
			      SDL_WINDOWPOS_CENTERED,
			      gWindow_size.x,
			      gWindow_size.y,
			      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(gWindow == NULL){
	printf("Failed on window creation\n");
	return -1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    if(gRenderer == NULL){
	printf("Failed on renderer creation\n");
	return -1;
    }

}

void
loop(void){

    //events();
    SDL_Event e;
    while(gIsGameRunning){

	while(SDL_PollEvent(&e)){
	    switch(e.type){
	    case SDL_QUIT:
		gIsGameRunning = false;
		break;
	    case SDL_WINDOWEVENT:
		//handling window events
		switch(e.window.event){
		case SDL_WINDOWEVENT_RESIZED:
		    SDL_Log("Window %d resized to %d x %d\n", e.window.windowID, e.window.data1, e.window.data2);
		    gWindow_size.x = e.window.data1;
		    gWindow_size.y = e.window.data2;

		    gWord_center.x = gWindow_size.x / 2;
		    gWord_center.y = gWindow_size.y / 2;
		    break;
		}
		break;
	    }
	}
	//logic();
	const int rsize = 20;

	SDL_Point mp;
	SDL_GetMouseState(&mp.x, &mp.y);
	
	//draw();
	SDL_SetRenderDrawColor(gRenderer, 100, 25, 75, 255);
	SDL_RenderClear(gRenderer);

	//draw window mouse pos
	SDL_Rect mwp = {mp.x - (rsize / 2), mp.y - (rsize / 2), rsize, rsize};
	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(gRenderer, &mwp); 

	//draw mouse word position
	SDL_Point wp = screen_to_word((SDL_Point){mwp.x, mwp.y});
	SDL_Rect mwwp = {wp.x, wp.y, rsize, rsize};
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(gRenderer, &mwwp); 

	//draw center
	SDL_Rect cr = {gWord_center.x - (rsize / 2), gWord_center.y - (rsize / 2), rsize, rsize};
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &cr); 
	
	SDL_RenderPresent(gRenderer);
    }    
}

void
load_resources(void){
    
}

void
free_resources(void){
    
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
