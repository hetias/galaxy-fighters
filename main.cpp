#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

bool initialize_sdl(void);
void game_loop();
void free_sdl(void);

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
bool g_isGameRunning = true;

int main(int argc, char* argv[]){

  //start game
  if(initialize_sdl()){
    return EXIT_FAILURE;
  }

  //game loop
  game_loop();

  
  return EXIT_SUCCESS;
}

void game_loop(){

  while(g_isGameRunning){
    //events
    SDL_Event e;
    while(SDL_PollEvent(&e)){

    
      switch(e.type){
      
      case SDL_QUIT:
	g_isGameRunning = false;
	break;
      
      }//end switch
    
    }//end while
  
    //input
    const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);

    if(KeyboardState[SDL_SCANCODE_ESCAPE]){
      g_isGameRunning = false;
    }

    //logic

    //render
    
  }

  
}

bool initialize_sdl(void){

  if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
    std::cout<<"coudlnt initialize sdl2: "<<SDL_GetError()<<std::endl;

    return EXIT_FAILURE;
  }

  g_window = SDL_CreateWindow("title",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
			      WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if(g_window == nullptr){
    std::cout<<"coudlnt crete window: "<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }

  g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED );
  if(g_renderer == nullptr){
    std::cout<<"coudlnt crete SDL2 renderer: "<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }

  if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
  {
    std::cout<<"coudlnt initialize sdl_image"<<std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void free_sdl(void){
  
}
