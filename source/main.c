#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"core/definitions.h"

#include"scene.h"
#include"gg.h"

typedef struct{
    SDL_Texture *textures[TXT_TOTAL];
    TTF_Font     *font;
    
}resources_t;

enum APP_STATES{
    MAIN_MENU = 0,
    OPTIONS_MENU,
    SCENARIO_LOAD,
    SCENARIO,
    SCENARIO_END,
    EXIT
};

resources_t game_resources = {};

int init();
void deinit();
void game_loop();

//menu options
void main_menu();
void options_menu();

//TTF_Font* gFont = NULL;
SDL_Renderer *renderer = NULL;
SDL_Window     *window = NULL;
bool    gIsGameRunning = true;
bool gPaused = false;
int gAppState = MAIN_MENU;

scene_t *game_scene    = NULL;

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



int main(void){

    if(init() < 0)
	exit(-1);
    
    game_loop();
    
    deinit();
    return 0;
}


    /* if(gAppState == APP_MAIN_MENU){ */
    /* 	//draw main menu */

    /* 	//get inputs */

    /* 	if(level_start_bt("START", font, collider)){ */
    /* 	    gAppState = APP_SCENE_START; */
    /* 	} */

    /* 	//scene */
	
    /* 	//draw */
    /* } */

    /* if(gAppState == APP_SCENE_START){ */
    /* 	scene_t* scene = scene_create(gTexturesPaths, renderer); */
    
    /* 	game_loop(scene); */

    /* 	scene_destroy(scene); */
    /* } */
    
    /* if(gAppState == APP_EXIT){ */
    /* 	//safe application exit */
    /* } */


void game_loop(void){

    //game running
    while(gIsGameRunning){	
	//event loop
	SDL_Event e;
	while(SDL_PollEvent(&e)){
	    switch(e.type){
	    case SDL_QUIT:
		gIsGameRunning = false;
		break;
	    case SDL_KEYUP:
		if(e.key.keysym.scancode == SDL_SCANCODE_P){
		    gPaused = !gPaused;
		}
		break;
	    }

	    gg_events(e);
	}
    
	//input
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if(keyboardState[SDL_SCANCODE_ESCAPE]){
	    gIsGameRunning = false;
	}
	
	//update
	if(gAppState == SCENARIO){
	    scene_update(game_scene);
	}
	
	//draw
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	switch(gAppState){
	case MAIN_MENU:{
	    printf("on main_menu\n");
	    //bg
	    	    
	    //create main menu
	    main_menu();
	}break;
	case OPTIONS_MENU:{
	    printf("on options_menu\n");	    
	    options_menu();
	}break;
	case SCENARIO:{
	    printf("on scenario\n");	    	    
	    scene_draw(game_scene, renderer);
	}
	case EXIT:{
	    printf("on exit\n");
	    gIsGameRunning = false;
	}
	}	
	
	//scene_draw(game_scene, renderer);
    
	SDL_RenderPresent(renderer);
	SDL_Delay(16);
    }
    
    scene_destroy(game_scene);
  
}


void main_menu(){
    int selected = -1;
    SDL_Point button_position = {0, 0};
    SDL_Rect button_rect = {0, 0, 120, 75};

    const char* options[] = {
	"Start",
	"Options",
	"Exit"
    };
	    
    gg_begin();
    //draw all available options
    for(int i = 0; i < 3; i++){

	//set rectangle
	SDL_Rect bt = {
	    button_rect.x, button_rect.y + (button_rect.h * i) + (15 * i),
	    button_rect.w, button_rect.h};

	//draw button
	if(gg_button(0, bt) ){
	    selected = i;
	}

	//draw label
	gg_label(bt, options[i], 16);
    }
    gg_end();

    //do based on selection
    switch(selected){
    case 0:{
	printf("Start\n");
	game_scene = scene_create(gTexturesPaths, renderer);
	gAppState = SCENARIO;
    }break;
    case 1:{
	printf("Options\n");
	options_menu();
	gAppState = OPTIONS_MENU;	
    }break;
    case 2:{
	printf("Exit\n");
	gAppState = EXIT;	
    }break;
    default:{}
    }
	        
}

void options_menu(){
    
}

int init(){
    int success = 0;

    //
    //INITIALIZE SDL
    //
    success = SDL_Init(SDL_INIT_EVERYTHING);
    if(success != 0){
	printf("Failed on initializing sdl2\n");
	return -1;
    }

    //
    //CREATE WINDOW
    //
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

    //
    //CREATE RENDERER
    //
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
	printf("Failed on renderer creation\n");
	SDL_Quit();
	SDL_DestroyWindow(window);
	return -1;
    }

    //
    //INITIALIZE SDL IMAGE
    //
    success = IMG_Init(IMG_INIT_PNG);
    if(success != IMG_INIT_PNG){
	printf("Failed on initalizing IMG\n");
	printf("flags: Returned: %d - Expected: %d\n", success, IMG_INIT_PNG);
	printf("%s\n", SDL_GetError());
    
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return -1;
    }

    //
    //INITIALIZE SDL TTF
    //
    success = TTF_Init();
    if(success < 0){
	printf("Failed initializing SDL_TTF\n");
	printf("%s\n", TTF_GetError());

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	IMG_Quit();	
	return -1;
    }

    gFont = TTF_OpenFont("resources/fonts/font.ttf", 16);
    
    if(gg_init(renderer, gFont) < 0){
	printf("gg_init failed!\n");
	return -1;
    }
    
    return 0;
}

void deinit(){
    gg_deinit();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
