#include<iostream>
#include<vector>
#include<list>
#include<string>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#include"player.hpp"
#include"enemy.hpp"
#include"spline.hpp"

bool initialize_sdl(void);
void game_loop(player_t*);
void free_sdl(void);

bool load_resources(void);
void free_resources(void);

void draw_text(const char*, SDL_Point, SDL_Color);

//GLOBAL VARS
TTF_Font* gFont = nullptr;
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
bool gIsGameRunning = true;

std::list<projectile_t*> gProjectiles;
std::vector<SDL_Texture*> gTextures;
std::vector<enemy_t*> gEnemies;

Uint32 scene_tick = 0;

const char* gTexturesPaths[] =
{
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

spline mySpline =
{
  //vector points
  {
    //x   //y
    {0.0f            , WINDOW_HEIGHT / 2},    
    {100.0f          , WINDOW_HEIGHT / 3},
    {200.0f          , WINDOW_HEIGHT / 3},
    {300.0f          , WINDOW_HEIGHT / 2}
  },
  //is looped?
  false
};


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
  player_t* bluePlayer = player_create();
  enemy_t*  enemy      = enemy_create();

  gEnemies.push_back(enemy);
  
  //
  //START GAME LOOP
  //
  game_loop(bluePlayer);

  //
  //DELETE ALL INTITIES AND OBJECTS
  //
  player_destroy(bluePlayer);

  for(auto _enemy : gEnemies)
    enemy_destroy(_enemy);
  gEnemies.clear();

  for(auto _prj : gProjectiles)
    projectile_destroy(_prj);
  gProjectiles.clear();

  
  //
  //FREE GAME RESOURCES
  //
  free_resources();
  
  return EXIT_SUCCESS;
}



//
//MAIN GAME LOOP
//
void game_loop(player_t* bluePlayer){

  for(auto point : mySpline.points){
    std::cout<<point.x<<" "<<point.y<<std::endl;
  }
  
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
    player_update(bluePlayer, KeyboardState);

    for(auto _enemy : gEnemies)
    {
      enemy_update(_enemy);
    }

    for(auto _prj = gProjectiles.begin(); _prj != gProjectiles.end(); _prj++)
    {
      if(projectile_update(*_prj)){
        _prj = gProjectiles.erase(_prj);
      }
    }
    
    scene_tick++;
    
    //frame end count
    Uint32 fps_end = SDL_GetTicks();
    
    Uint32 fps_elapsed = fps_end - fps_start;

    fps_start = fps_elapsed;
    
    //render
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);

    /*background elements*/
    SDL_RenderCopy(gRenderer, gTextures.at(TXT_BG_DARKPURPLE), NULL, NULL);

    /*important elements*/
    spline_draw(mySpline);
    
    player_draw(bluePlayer);

    for(auto enemy : gEnemies)
    {
      enemy_draw(enemy);
    }

    for(auto prj : gProjectiles)
    {
      projectile_draw(prj);
    }
    
    /*UI elements */
    draw_text(std::to_string(scene_tick).c_str(), {0, 0}, {255, 255, 255});

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



//
//LOAD ALL GAME RESOURCES
//
bool load_resources(void){
  SDL_Texture* _tmpTexture = nullptr;

  //OPEN GAME FONT
  gFont = TTF_OpenFont("../resources/fonts/font.ttf", 24);
  if (!gFont){
    std::cout<<SDL_GetError()<<std::endl;
    return EXIT_FAILURE;
  }

  //
  //LOOAD TEXTURES
  //

  for(auto path : gTexturesPaths)
  {
    _tmpTexture = IMG_LoadTexture(gRenderer,
                                  path);
    if(!_tmpTexture){
      std::cout<<SDL_GetError()<<std::endl;
      return EXIT_FAILURE;
    }
    gTextures.push_back(_tmpTexture);
  }

  
  return EXIT_SUCCESS;
}

void free_resources(){

  TTF_CloseFont(gFont);
  
  for(auto it : gTextures){
    SDL_DestroyTexture(it);
    it = nullptr;
  }
  
  gTextures.clear();
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
