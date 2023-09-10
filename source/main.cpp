#include<iostream>
#include<vector>
#include<list>
#include<string>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#include"scene.hpp"

bool initialize_sdl(void);
void game_loop(scene_t*);
void free_sdl(void);

void draw_text(const char*, SDL_Point, SDL_Color);

//GLOBAL VARS
TTF_Font* gFont = nullptr;
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
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
  "../resources/sprites/lasers/laserRed.png"
};

//
//MAIN FUNCTION
//
int main(int argc, char* argv[]){

  if( initialize_sdl() ){
    return EXIT_FAILURE;
  }

  scene_t* game_scene = scene_create(gTexturesPaths);

  game_loop(game_scene);
  
  scene_destroy(game_scene);
  
  return EXIT_SUCCESS;
}



//
//MAIN GAME LOOP
//
void game_loop(scene_t* _scene){
  
  //fps count start
  Uint32 fps_start = SDL_GetTicks();
    
  while(gIsGameRunning){

    //events
    SDL_Event e;
    while(SDL_PollEvent(&e)){

    
      switch(e.type){
      
      case SDL_QUIT:
	gIsGameRunning = false;
	break;
      
      }//end switch
    
    }//end while
  
    //input
    const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);

    if(KeyboardState[SDL_SCANCODE_ESCAPE]){
      gIsGameRunning = false;
    }

    //update

    scene_update(_scene);
        
    //frame end count
    Uint32 fps_end = SDL_GetTicks();
    
    Uint32 fps_elapsed = fps_end - fps_start;

    fps_start = fps_elapsed;
    
    //render
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);
    
    scene_draw(_scene, gRenderer);

    SDL_RenderPresent(gRenderer);

    //fps capping
    SDL_Delay(floor(16.666f - (fps_elapsed / 1000.0f)));
    
  }

  
}



//INITIALIZE SDL2 AND OTHER LIBRARIES
bool initialize_sdl(void){
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
    std::cout<<"coudlnt initialize sdl2: "<<SDL_GetError()<<std::endl;

    return EXIT_FAILURE;
  }

  gWindow = SDL_CreateWindow("Galaxy Fighters",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
			      WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
  if(gWindow == nullptr){
    std::cout<<"coudlnt crete window: "<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED );
  if(gRenderer == nullptr){
    std::cout<<"coudlnt crete SDL2 renderer: "<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }

  if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
  {
    std::cout<<"coudlnt initialize sdl_image"<<std::endl;
    return EXIT_FAILURE;
  }

  if(TTF_Init() < 0)
  {
    std::cout<<"couldnt initiazlie sdl_ttf"<<std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}



//
//FREE SDL2 OBJECTS AND QUIT SDL2 SUBSYSTEMS AND LIBS
//
void free_sdl(void){
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
}



void draw_text(const char* _text, SDL_Point _position, SDL_Color _color){

  SDL_Surface* text_surface = NULL;
  SDL_Texture* text_texture = NULL;

  text_surface = TTF_RenderText_Solid(gFont, _text, _color );
  if(text_surface){
    text_texture = SDL_CreateTextureFromSurface(gRenderer, text_surface);
    if(text_surface){
      SDL_Rect dstrect = {_position.x, _position.y, text_surface->w, text_surface->h};
      SDL_RenderCopy(gRenderer, text_texture, NULL, &dstrect );

      SDL_FreeSurface(text_surface);
      SDL_DestroyTexture(text_texture);
    }
    
  }
  
}
