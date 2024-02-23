#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"core/definitions.h"

#include"resources.h"
#include"scene.h"
#include"gg.h"

enum APP_STATES{
    MAIN_MENU = 0,
    OPTIONS_MENU,
    SCENARIO_LOAD,
    SCENARIO,
    SCENARIO_END,
    EXIT
};

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

    load_resources();
    
    game_loop();

    free_resources();
    
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

/**
 * Main game loop of the application.
 * Menus and gameplay logic happens here.
 */
void game_loop(void){

    gg_init(renderer, game_resources.font);
    
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
    
	//keyboard input
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

	//we do depending on the Global Application State
	switch(gAppState){
	case MAIN_MENU:{
	    //bg
	    	    
	    //create main menu
	    main_menu();
	}break;
	case OPTIONS_MENU:{
	    options_menu();
	}break;
	case SCENARIO:{
	    scene_draw(game_scene, renderer);
	}break;
	case EXIT:{
	    gIsGameRunning = false;
	}break;
	}	
	
	//scene_draw(game_scene, renderer);
    
	SDL_RenderPresent(renderer);
	SDL_Delay(16);
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
