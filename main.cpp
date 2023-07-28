#include<iostream>
#include<vector>
#include<string>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#include"player.hpp"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

typedef struct enemy_s{
  SDL_FPoint position;
  SDL_FRect hitbox;
  SDL_Texture* sprite;
  int hp;
}enemy_t;

enemy_t* enemy_create(SDL_Texture*);
void enemy_update(enemy_t*);
void enemy_draw(enemy_t*);

enum ETextures{
  //background
  TXT_BG_BLACK,
  TXT_BG_PURPLE,
  TXT_BG_BLUE,
  TXT_BG_DARKPURPLE,
  //player
  TXT_PLAYER_BLUE,
  //enemy
  TXT_ENEMY1_BLACK,
  //vector final size
  TXT_VECSIZE
};

const char* gTexturesPaths[] =
{
  "resources/sprites/backgrounds/black.png",
  "resources/sprites/backgrounds/purple.png",
  "resources/sprites/backgrounds/blue.png",
  "resources/sprites/backgrounds/darkPurple.png",
  "resources/sprites/player/player_blue.png",
  "resources/sprites/enemies/enemyBlack1.png"
};

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

std::vector<SDL_Texture*> gTextures;
std::vector<enemy_t*> gEnemies;

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
  player_t* bluePlayer = player_create(gTextures.at(TXT_PLAYER_BLUE));
  enemy_t*  enemy      = enemy_create(gTextures.at(TXT_ENEMY1_BLACK));

  gEnemies.push_back(enemy);
  
  //
  //START GAME LOOP
  //
  game_loop(bluePlayer);

  //
  //DELETE ALL INTITIES AND OBJECTS
  //
  player_destroy(bluePlayer);

  for(auto enemy : gEnemies)
  {
    delete enemy;
    enemy = NULL;
  }
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
  
  while(gIsGameRunning){
    //fps count start
    Uint32 fps_start = SDL_GetTicks();

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

    for(auto enemy : gEnemies)
    {
      enemy_update(enemy);
    }
    
    scene_tick++;
    
    //frame end count
    Uint32 fps_end = SDL_GetTicks();
    
    double fps_elapsed = double(fps_end - fps_start) / 1000.0f;
    
    //render
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);

    /*background elements*/
    SDL_RenderCopy(gRenderer, gTextures.at(TXT_BG_DARKPURPLE), NULL, NULL);

    /*important elements*/
    player_draw(bluePlayer);

    for(auto enemy : gEnemies)
    {
      enemy_draw(enemy);
    }

    /*UI elements */
    draw_text(std::to_string(scene_tick).c_str(), {0, 0}, {255, 255, 255});

    SDL_RenderPresent(gRenderer);

    //fps capping
    SDL_Delay(floor(16.666f - fps_elapsed));
    
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
  gFont = TTF_OpenFont("resources/fonts/font.ttf", 24);
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

    //std::cout<<path<<std::endl;
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

enemy_t* enemy_create(SDL_Texture* _enemyTexture)
{
  if(!_enemyTexture) return nullptr;

  enemy_t* _tmp = new enemy_t;
  
  _tmp->position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

    //get texture dimensions
  SDL_Point dimensions = {0};
  SDL_QueryTexture(_enemyTexture,
                   NULL,
                   NULL,
                   &dimensions.x,
                   &dimensions.y);

  _tmp->hitbox   = {_tmp->position.x,
                  _tmp->position.y,
                  (float)dimensions.x,
                  (float)dimensions.y};
  _tmp->sprite   = _enemyTexture;
  _tmp->hp       = 4;

  return _tmp;  
}

void enemy_update(enemy_t* _enemy)
{
  _enemy->hitbox.x = _enemy->position.x - _enemy->hitbox.w / 2;
  _enemy->hitbox.y = _enemy->position.y - _enemy->hitbox.h / 2;
}

void enemy_draw(enemy_t* _enemy)
{  
  if(_enemy){

    SDL_RenderCopyF(gRenderer,
                   _enemy->sprite,
                   NULL,
                   &_enemy->hitbox
                   );
  }
  /*
   
  SDL_FRect _positionSquare = {_enemy->position.x - 5.0f,
    _enemy->position.y - 5.0f,
    10.0f,
    10.0f,
  };
  
  //draw centered cube
  SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
  SDL_RenderDrawRectF(gRenderer, &_positionSquare);

  //draw hitbox
  SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
  SDL_RenderDrawRectF(gRenderer, &_enemy->hitbox);

  //draw position point
  SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
  SDL_RenderDrawPointF(gRenderer, _enemy->position.x, _enemy->position.y);
  */
  
}
