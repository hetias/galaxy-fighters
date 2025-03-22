#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

//SDL2 Multimedia Library
#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"

//Nuklear Immediate Mode GUI library
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION

//Game Internals
#include"editor.h"
#include"input.h"

#include"core/global.h"
#include"core/debug_draw.h"
#include"core/definitions.h"
#include"core/resources.h"
#include"core/scene.h"
#include"core/timing.h"

enum APP_STATES{
    MAIN_MENU = 0,
    OPTIONS_MENU,
    SCENARIO_LOAD,
    SCENARIO,
    SCENARIO_END,
    EDITOR_START,
    EDITOR_RUNNING,
    EXIT
};

struct main_menu_bgfx{
    int position_1;
    int position_2;
};

struct main_menu_bgfx mm_bgfx = {-600, 0};

int init(void);
void deinit(void);
void load_resources(void);
void free_resources(void);
void game_loop(void);

//menu options
int main_menu(void);
int options_menu(void);

//global structures
debug_info_t g_debug_info;
resources_t game_resources;
struct nk_context *gui_context;
struct nk_colorf bg;
SDL_Renderer *renderer = NULL;
SDL_Window     *window = NULL;
bool    gIsGameRunning = true;
bool           gPaused = false;
int          gAppState = MAIN_MENU;

global_state g_state;

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
void game_loop(){

    SDL_Event e;

    //main loop
    while(gIsGameRunning){

	timeframe_start(&gametime);

	nk_input_begin(gui_context);
	while(SDL_PollEvent(&e)){
	    switch(e.type){
	    case SDL_QUIT:
		gAppState = EXIT;
		break;
	    case SDL_KEYUP:
		SDL_Scancode code = e.key.keysym.scancode;
		if(code == SDL_SCANCODE_P)
		    gPaused = !gPaused;
		if(code == SDL_SCANCODE_ESCAPE && gAppState == OPTIONS_MENU)
		    gAppState = MAIN_MENU;
		if(code == SDL_SCANCODE_E && gAppState == MAIN_MENU)
		    gAppState = EDITOR_START;
		break;
	    }

	    if(gAppState == EDITOR_RUNNING)
		editor_events(e);
	    nk_sdl_handle_event(&e);
	}
	nk_sdl_handle_grab();
	nk_input_end(gui_context);

	const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	switch(gAppState){
	case MAIN_MENU:{
	    int selection = main_menu();
	    if(selection != -1) gAppState = selection;
	}break;
	case OPTIONS_MENU:{
	    int selection = options_menu();
	    if(selection != -1) gAppState = selection;
	}break;
	case SCENARIO_LOAD:
	    game_scene = scene_create(&game_resources, "level.lvl");
	    if(game_scene == NULL){
		printf("failed load of scene\n");
		return;
	    }
	    gAppState = SCENARIO;
	    break;
	case SCENARIO:
	    scene_update(game_scene);
	    scene_draw(game_scene, renderer);
	    break;
	case EXIT:
	    gIsGameRunning = false;
	    break;
	case EDITOR_START:
	    editor_start(&game_resources, window);
	    gAppState = EDITOR_RUNNING;
	    break;
	case EDITOR_RUNNING:
	    editor_running(gui_context);
	    editor_draw(renderer);
	}

	debug_draw_present();
	nk_sdl_render(NK_ANTI_ALIASING_ON);
	SDL_RenderPresent(renderer);

	timeframe_end(&gametime);

	Uint64 elapsed = time_get_elapsed(&gametime);

	if(elapsed < 16)
	    SDL_Delay(16 - elapsed);

    }
    scene_destroy(game_scene);
}

/**
 *
 * Draws the main menu of the game. From there we can start
 * the game, go to options or exit.
 *
 */
int main_menu(void){
    int selection = -1;
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

    if(nk_begin(gui_context, "", nk_rect(250, 150, 120, 250), 0)){
	nk_layout_row_static(gui_context, 32, 100, 1);
	if(nk_button_label(gui_context, "start")){
	    selection = SCENARIO_LOAD;
	}

	if(nk_button_label(gui_context, "options")){
	    selection = OPTIONS_MENU;
	}

	if(nk_button_label(gui_context, "exit")){
	    selection = EXIT;
	}
    }
    nk_end(gui_context);
    return selection;
}

/**
 * Draw the options menu.
 */
int options_menu(void){
    int selection = -1;
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

    //GUI
    if(nk_begin(gui_context, "", nk_rect(250, 150, 120, 250), 0)){
	nk_layout_row_static(gui_context, 32, 100, 1);
	if(nk_button_label(gui_context, "option 1")){

	}

	if(nk_button_label(gui_context, "option 2")){

	}

	if(nk_button_label(gui_context, "option 3")){

	}

	if(nk_button_label(gui_context, "option 3")){
	    selection = MAIN_MENU;
	}
    }
    nk_end(gui_context);

    return selection;
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

    g_state.renderer = NULL;
    g_state.window = NULL;
    g_state.scene = NULL;
    g_state.time = (time_info){0, 0, 0};
    g_state.debug_info = (debug_info_t){.text_texture = NULL, .text_surface = NULL};
    g_state.resources = (resources_t){.font = NULL};
    g_state.is_running = true;
    g_state.is_paused = false;
    g_state.app_state = MAIN_MENU;
    g_state.gui_context = NULL;

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


    //
    // Initializes Nuklear
    //
    gui_context = nk_sdl_init(window, renderer);
    {
	float font_scale = 1.0;
	struct nk_font_atlas *atlas;
	struct nk_font_config config = nk_font_config(0);
	struct nk_font *font;

	nk_sdl_font_stash_begin(&atlas);
	font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
	nk_sdl_font_stash_end();

	font->handle.height /= font_scale;

	nk_style_set_font(gui_context, &font->handle);
    }
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    debug_start();

    return 0;
}

/**
 * Closes all SDL2 libraries and destroys resources.
 */
void deinit(void){
    debug_clean();
    nk_sdl_shutdown();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
