#include<iostream>
#include<vector>
#include<string>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#include"player.hpp"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

enum ETextures{
  TXT_BG_BLACK,
  TXT_BG_PURPLE,
  TXT_BG_BLUE,
  TXT_BG_DARKPURPLE,
  TXT_PLAYER_BLUE,
  TXT_VECSIZE
};

bool initialize_sdl(void);
void game_loop(player_t*);
void free_sdl(void);

bool load_resources(void);
void free_resources(void);

void draw_text(const char*, SDL_Point, SDL_Color);

//GLOBAL VARS
TTF_Font* g_font = nullptr;
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
bool g_isGameRunning = true;

std::vector<SDL_Texture*> g_textures;

Uint32 scene_tick = 0;


//
//MAIN FUNCTION
//
int main(int argc, char* argv[]){

  //
  //START SDL AND SDL EXTENSIONS
  //
  if( initialize_sdl() ){
    return EXIT_FAILURE;
  }

  //
  //LOAD ALL GAMES RESOURCES
  //
  if( load_resources() ){
    return EXIT_FAILURE;
  }

  //
  //CREATE NEEDED ENTITIES AND OBJECTS
  //
  player_t* bluePlayer = player_create(g_textures.at(TXT_PLAYER_BLUE));

  //
  //START GAME LOOP
  //
  game_loop(bluePlayer);

  //
  //DELETE ALL INTITIES AND OBJECTS
  //
  player_destroy(bluePlayer);

  //
  //FREE GAME RESOURCES
  //
  free_resources();
  
  return EXIT_SUCCESS;
}

void game_loop(player_t* bluePlayer){
  
  while(g_isGameRunning){
    //fps count start
    Uint32 fps_start = SDL_GetTicks();

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

    //player
    player_update(bluePlayer, KeyboardState);

    scene_tick++;
    
    //frame end count
    Uint32 fps_end = SDL_GetTicks();
    
    double fps_elapsed = double(fps_end - fps_start) / 1000.0f;
    
    //render
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0);
    SDL_RenderClear(g_renderer);

    /*background elements*/
    SDL_RenderCopy(g_renderer, g_textures.at(TXT_BG_DARKPURPLE), NULL, NULL);

    /*important elements*/
    player_draw(bluePlayer);

    /*UI elements */
    draw_text(std::to_string(scene_tick).c_str(), {0, 0}, {255, 255, 255});

    SDL_RenderPresent(g_renderer);

    //fps capping
    SDL_Delay(floor(16.666f - fps_elapsed));
    
  }

  
}

bool initialize_sdl(void){
  //
  //INITIALIZE SDL EVERYTHING
  //
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
    std::cout<<"coudlnt initialize sdl2: "<<SDL_GetError()<<std::endl;

    return EXIT_FAILURE;
  }


  
  //
  //CREATE WINDOW
  //
  g_window = SDL_CreateWindow("Galaxy Fighters",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
			      WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
  if(g_window == nullptr){
    std::cout<<"coudlnt crete window: "<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }


  
  //
  //CREATE RENDERER
  //    
  g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED );
  if(g_renderer == nullptr){
    std::cout<<"coudlnt crete SDL2 renderer: "<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }


  
  //
  //INITIALIZE SDL IMAGE
  //
  if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
  {
    std::cout<<"coudlnt initialize sdl_image"<<std::endl;
    return EXIT_FAILURE;
  }



  //
  //INITIALIZE SDL TTF
  //
  if(TTF_Init() < 0)
  {
    std::cout<<"couldnt initiazlie sdl_ttf"<<std::endl;
    return EXIT_FAILURE;
  }


  
  return EXIT_SUCCESS;
}

void free_sdl(void){
  SDL_DestroyRenderer(g_renderer);
  SDL_DestroyWindow(g_window);
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
}

bool load_resources(void){
  SDL_Texture* tmp_texture = nullptr;

  //OPEN GAME FONT
  g_font = TTF_OpenFont("resources/fonts/font.ttf", 24);
  if (!g_font){
    std::cout<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }

  //
  //LOOAD TEXTURES
  //
  tmp_texture = IMG_LoadTexture(g_renderer, "resources/sprites/backgrounds/black.png");
  if(!tmp_texture){
    std::cout<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }
  g_textures.push_back(tmp_texture);
  tmp_texture = nullptr;

  tmp_texture = IMG_LoadTexture(g_renderer, "resources/sprites/backgrounds/purple.png");
  if(!tmp_texture){
    std::cout<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }
  g_textures.push_back(tmp_texture);
  tmp_texture = nullptr;

  tmp_texture = IMG_LoadTexture(g_renderer, "resources/sprites/backgrounds/blue.png");
  if(!tmp_texture){
    std::cout<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }
  g_textures.push_back(tmp_texture);
  tmp_texture = nullptr;

  tmp_texture = IMG_LoadTexture(g_renderer, "resources/sprites/backgrounds/darkPurple.png");
  if(!tmp_texture){
    std::cout<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }
  g_textures.push_back(tmp_texture);
  tmp_texture = nullptr;

    tmp_texture = IMG_LoadTexture(g_renderer, "resources/sprites/player/player_blue.png");
  if(!tmp_texture){
    std::cout<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }
  g_textures.push_back(tmp_texture);
  tmp_texture = nullptr;
  
  return EXIT_SUCCESS;
}

void free_resources(){

  TTF_CloseFont(g_font);
  
  for(auto it : g_textures){
    SDL_DestroyTexture(it);
    it = nullptr;
  }
  
  g_textures.clear();
}

void draw_text(const char* _text, SDL_Point _position, SDL_Color _color){

  SDL_Surface* text_surface = NULL;
  SDL_Texture* text_texture = NULL;

  text_surface = TTF_RenderText_Solid(g_font, _text, _color );
  if(text_surface){
    text_texture = SDL_CreateTextureFromSurface(g_renderer, text_surface);
    if(text_surface){
      SDL_Rect dstrect = {_position.x, _position.y, text_surface->w, text_surface->h};
      SDL_RenderCopy(g_renderer, text_texture, NULL, &dstrect );

      SDL_FreeSurface(text_surface);
      SDL_DestroyTexture(text_texture);
    }
    
  }
  
}

