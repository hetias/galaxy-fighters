#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"core/definitions.h"

#include"core/resources.h"
#include"core/scene.h"
#include "core/gg.h"
#include "core/timing.h"

enum APP_STATES{
    MAIN_MENU = 0,
    OPTIONS_MENU,
    SCENARIO_LOAD,
    SCENARIO,
    SCENARIO_END,
    EXIT
};

struct main_menu_bgfx{
    int position_1;
    int position_2;
};

struct main_menu_bgfx mm_bgfx = {-600, 0};

resources_t game_resources;

int init(void);
void deinit(void);
void load_resources(void);
void free_resources(void);
void game_loop(void);

//menu options
void main_menu(void);
void options_menu(void);

SDL_Renderer *renderer = NULL;
SDL_Window     *window = NULL;
bool    gIsGameRunning = true;
bool           gPaused = false;
int          gAppState = MAIN_MENU;

//
time_info gametime = {0};
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

//TTF_Font *gFont = NULL;

int main(void){

    if(init() < 0)
	exit(-1);

    load_resources();
    
    game_loop();

    free_resources();
    
    deinit();
    return 0;
}

/**
 * Main game loop of the application.
 * Menus and gameplay logic happens here.
 */
void game_loop(void){
    
    gg_init(renderer, game_resources.font);
    
    //game running
    while(gIsGameRunning){
	
	timeframe_start(&gametime);
	
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
		
		if(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE
		   && gAppState == OPTIONS_MENU){
		    gAppState = MAIN_MENU;
		}
		break;
	    }
	    gg_events(e);
	}
    
	//keyboard input
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if(keyboardState[SDL_SCANCODE_ESCAPE] &&
	   gAppState == MAIN_MENU){
	    gIsGameRunning = false;
	};
	
	//update
	if(gAppState == SCENARIO){
	    scene_update(game_scene);
	}
	
	//draw
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	//we do depending on the Global Application State
	switch(gAppState){
	case MAIN_MENU:{
	    //bg
	    SDL_Rect dst_1 = {mm_bgfx.position_1, 0, 600, 600};
	    SDL_Rect dst_2 = {mm_bgfx.position_2, 0, 600, 600};
	    SDL_RenderCopy(renderer, game_resources.textures[TXT_BG_PURPLE], NULL, &dst_1);
	    SDL_RenderCopy(renderer, game_resources.textures[TXT_BG_PURPLE], NULL, &dst_2);

	    mm_bgfx.position_1 += 1;
	    mm_bgfx.position_2 += 1;

	    if(mm_bgfx.position_2 >= 600)
		mm_bgfx.position_2 = -600;
	    if(mm_bgfx.position_1 >= 600)
		mm_bgfx.position_1 = -600;		    
	    //create main menu
	    main_menu();
	}break;
	case OPTIONS_MENU:{
	    //bg
	    SDL_Rect dst_1 = {mm_bgfx.position_1, 0, 600, 600};
	    SDL_Rect dst_2 = {mm_bgfx.position_2, 0, 600, 600};
	    SDL_RenderCopy(renderer, game_resources.textures[TXT_BG_BLACK], NULL, &dst_1);
	    SDL_RenderCopy(renderer, game_resources.textures[TXT_BG_BLACK], NULL, &dst_2);

	    mm_bgfx.position_1 += 1;
	    mm_bgfx.position_2 += 1;

	    if(mm_bgfx.position_2 >= 600)
		mm_bgfx.position_2 = -600;
	    if(mm_bgfx.position_1 >= 600)
		mm_bgfx.position_1 = -600;		    	    
	    //options
	    options_menu();
	}break;
	case SCENARIO:{
	    scene_draw(game_scene, renderer);
	}break;
	case EXIT:{
	    gIsGameRunning = false;
	}break;
	}	
			
	SDL_RenderPresent(renderer);
	
        timeframe_end(&gametime);
	
	timeframe_cycles(&gametime);
	timeframe_ms(&gametime);
	timeframe_sc(&gametime);

	printf("cycles: %ld\n", gametime.frame_cycles);
	printf("ms: %f\n", gametime.frame_ms);
	printf("sc: %f\n", gametime.frame_sc);	
    }

    scene_destroy(game_scene);
    gg_deinit();    
}

/**
 * 
 * Draws the main menu of the game. From there we can start
 * the game, go to options or exit.
 *
*/
void main_menu(void){
    int selected = -1;
    SDL_Rect btn_rect = {300, 150, 120, 75}; //this is the first button, from here we arrange the rest

    const char* options[] = {
	"Start",
	"Options",
	"Exit"
    };
	    
    gg_begin();
    //we can't assing the rect neither know it's dimensions
    //(TODO::)make it so we can assing the rect(dimensions) OR know it's actual size
    gg_label( (SDL_Rect){300 - 115, 25, 0, 0}, "Galaxy Fighters", 28);
    //draw all available options
    for(int i = 0; i < 3; i++){

	//set rectangle
	SDL_Rect bt = {
	    btn_rect.x - btn_rect.w / 2,
	    btn_rect.y + (btn_rect.h * i) + (15 * i),
	    btn_rect.w,
	    btn_rect.h};

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
	game_scene = scene_create(&game_resources);
	gAppState = SCENARIO;
    }break;
    case 1:{
	gAppState = OPTIONS_MENU;	
    }break;
    case 2:{
	gAppState = EXIT;	
    }break;
    default:{}
    }
	        
}

/**
 * Draw the options menu.
 */
void options_menu(void){

    SDL_Point btn_pos = {300, 150};
    SDL_Point btn_dim = {150, 75};
    int gap = 10;

    const char *options[] = {
	"option_1",
	"option_2",
	"option_3",
    };

    gg_begin();       
    for(int i = 0; i < 3; i++){
	SDL_Rect btn = {
	    btn_pos.x - btn_dim.x / 2,
	    btn_pos.y + (btn_dim.y * i+1) + gap * i,
	    btn_dim.x,
	    btn_dim.y};
	gg_button(0, btn);
	gg_label(btn, options[i], 16);	
    }
    gg_end();	
}

/**
 * Load all game resources on a global 'resources_t' structure.
 */
void
load_resources(void){
    //load textures
    for(int i = 0; i < TXT_TOTAL; i++){
	game_resources.textures[i] = NULL;
	game_resources.textures[i] =
	    IMG_LoadTexture(renderer, gTexturesPaths[i]);
	if(game_resources.textures[i] == NULL){
	    printf("Warning: failed on resources load: %s\n",
		   gTexturesPaths[i]);
	}
    }

    //load font
    game_resources.font = TTF_OpenFont("resources/fonts/font.ttf", 16);
    if(game_resources.font == NULL){
	printf("Warning: failed on resource load: resources/fonts/font.ttf");
    }
}

/**
 * Free all game resources from global 'resources_t' structure.
 */
void
free_resources(void){
    //destroy all textures
    for(int i = 0; i < TXT_TOTAL; i++){
	SDL_DestroyTexture(game_resources.textures[i]);
	game_resources.textures[i] = NULL;
    }

    //close font
    TTF_CloseFont(game_resources.font);
}

/**
 * Initializes SDL2, SDL2_Image, SDL_ttf create windows and 
 * renderer.
 * @return returns 0 on succes or -1 on error
 */
int init(void){
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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
    
    return 0;
}

/**
 * Closes all SDL2 libraries and destroys resources.
 */
void deinit(void){        
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
