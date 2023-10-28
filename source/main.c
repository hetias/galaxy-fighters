#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"definitions.h"

#include"scene.h"

int init();
void deinit();
void game_loop(scene_t*);

//TTF_Font* gFont = NULL;
SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
bool gIsGameRunning = true;

const char* gTexturesPaths[] ={
    //background
    "../resources/sprites/backgrounds/black.png",
    "../resources/sprites/backgrounds/purple.png",
    "../resources/sprites/backgrounds/blue.png",
    "../resources/sprites/backgrounds/darkPurple.png",
    //Player
    "../resources/sprites/player/player_blue.png",
    //Enemy
    "../resources/sprites/enemies/enemyBlack1.png",
    //Lasers
    "../resources/sprites/lasers/laserBlue.png",
    "../resources/sprites/lasers/laserRed.png",
    //ui
    "../resources/sprites/ui/lifeblue1.png"
};

int main(void){

    if(init() < 0)
	exit(-1);

    scene_t* scene = scene_create(gTexturesPaths, renderer);
  
    game_loop(scene);

    scene_destroy(scene);
  
    deinit();
    return 0;
}

void game_loop(scene_t* game_scene){

    int curr = 0;

    Uint64 delta_now = SDL_GetPerformanceCounter();    
    Uint64 delta_last = 0;
    double delta_time = 0.0f;
    
    //game running
    while(gIsGameRunning){
	delta_last = delta_now;
	delta_now = SDL_GetPerformanceCounter();

	delta_time = (double)(delta_now - delta_last)*1000 / (double)SDL_GetPerformanceFrequency();
	
	//event loop
	SDL_Event e;
	while(SDL_PollEvent(&e)){
	    switch(e.type){
	    case SDL_QUIT:
		gIsGameRunning = false;
	    }
	}
    
	//input
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if(keyboardState[SDL_SCANCODE_ESCAPE]){
	    gIsGameRunning = false;
	}

	//update
	scene_update(game_scene);
    
	//draw
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	scene_draw(game_scene, renderer);
    
	SDL_RenderPresent(renderer);
	SDL_Delay(16);
    }
  
}

int init(){
    int success = 0;

    success = SDL_Init(SDL_INIT_EVERYTHING);
    if(success != 0){
	printf("Failed on initializing sdl2\n");
	return -1;
    }
  
    window = SDL_CreateWindow("window",
			      SDL_WINDOWPOS_CENTERED,
			      SDL_WINDOWPOS_CENTERED,
			      WINDOW_WIDTH,
			      WINDOW_HEIGHT,
			      SDL_WINDOW_SHOWN);
    if(window == NULL){
	printf("Failed on window creation\n");
	SDL_Quit();
	return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
	printf("Failed on renderer creation\n");
	SDL_Quit();
	SDL_DestroyWindow(window);
	return -1;
    }

    success = IMG_Init(IMG_INIT_PNG);
    if(success != IMG_INIT_PNG){
	printf("Failed on initalizing IMG\n");
	printf("flags: Returned: %d - Expected: %d\n", success, IMG_INIT_PNG);
	printf("%s\n", SDL_GetError());
    
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
    }

    return 0;
}

void deinit(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
