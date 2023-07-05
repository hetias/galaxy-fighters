#include<iostream>
#include<vector>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

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

typedef struct player_s{
  SDL_FPoint position;
  SDL_FPoint direction;
  SDL_FRect hitbox;
  float speed;
  SDL_Texture* sprite;
}player_t;

player_t* player_create();
void player_update(player_t*, const Uint8*);
void player_draw(player_t*);

bool initialize_sdl(void);
void game_loop(player_t*);
void free_sdl(void);

bool load_resources(void);
void free_resources(void);

std::vector<SDL_Texture*> g_textures;

player_t player_ship;

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
bool g_isGameRunning = true;

int main(int argc, char* argv[]){

  //start game
  if(initialize_sdl()){
    return EXIT_FAILURE;
  }
  
  load_resources();
  
  player_t* bluePlayer = player_create();
  
  game_loop(bluePlayer);

  delete bluePlayer;

  free_resources();
  
  return EXIT_SUCCESS;
}

void game_loop(player_t* bluePlayer){

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

    player_update(bluePlayer, KeyboardState);
    
    //render
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0);
    SDL_RenderClear(g_renderer);

    player_draw(bluePlayer);
    
    //SDL_RenderCopy(g_renderer, g_textures.at(TXT_BG_DARKPURPLE), NULL, NULL);

    SDL_RenderPresent(g_renderer);
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
  SDL_DestroyRenderer(g_renderer);
  SDL_DestroyWindow(g_window);
  SDL_Quit();
  IMG_Quit();
}

bool load_resources(void){
  SDL_Texture* tmp_texture = nullptr;

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
  for(auto it : g_textures){
    SDL_DestroyTexture(it);
    it = nullptr;
  }
  g_textures.clear();
}

player_t* player_create(){
  player_t* tmp_player = new player_t();
  tmp_player->position = {300.0f, 300.0f};
  tmp_player->direction = {0};
  tmp_player->hitbox = {0.0f, 0.0f, 99.0f, 75.0f};  
  tmp_player->speed = 5.0f;
  tmp_player->sprite = g_textures.at(TXT_PLAYER_BLUE);

  return tmp_player;
}

void player_update(player_t* _player, const Uint8* _keyboardState){
  //get inputs
  if(_keyboardState[SDL_SCANCODE_W]){
    if(_player->direction.y >= 0){
      _player->direction.y -= 1;
    }
  }else if(_keyboardState[SDL_SCANCODE_S]){
    if(_player->direction.y <= 0){
      _player->direction.y += 1;
    }
  }else{
    _player->direction.y = 0;
  }

  if(_keyboardState[SDL_SCANCODE_A]){
    if(_player->direction.x >= 0){
      _player->direction.x -= 1;
    }
  }else if(_keyboardState[SDL_SCANCODE_D]){
    if(_player->direction.x <= 0){
      _player->direction.x  += 1;
    }
  }else{
    _player->direction.x = 0;
  }

    
  if(_keyboardState[SDL_SCANCODE_J]){
    //shoot projectile
  }

  //move
  _player->position.x += _player->direction.x * _player->speed;
  _player->position.y += _player->direction.y * _player->speed;

  _player->hitbox.x = _player->position.x;
  _player->hitbox.y = _player->position.y;
}

void player_draw(player_t* _player){
  if(_player->sprite){
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 255);
    SDL_RenderDrawPointF(g_renderer, _player->position.x,_player->position.y);

    SDL_SetRenderDrawColor(g_renderer, 255, 0, 0 ,255);
    SDL_RenderDrawRectF(g_renderer, &_player->hitbox);

    SDL_RenderCopyF(g_renderer, _player->sprite, NULL, &_player->hitbox);
  }
}
