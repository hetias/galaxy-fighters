#include"player.hpp"



//
//PLAYER CREATION
//
player_t* player_create(){
  player_t* tmp_player = new player_t();
  tmp_player->hp        = 5;
  tmp_player->position  = {300.0f, 300.0f};
  tmp_player->direction = {0};
  tmp_player->hitbox    = {0.0f, 0.0f, 99.0f, 75.0f};  
  tmp_player->speed     = 5.0f;
  tmp_player->sprite    = g_textures.at(player_texture);

  return tmp_player;
}



//
//PLAYER LOGIC UPDATE
//
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


  //normalizing vector
  float magnitud = sqrt(_player->direction.x * _player->direction.x + _player->direction.y * _player->direction.y);
  _player->direction.x = (_player->direction.x != 0.0) ? _player->direction.x / magnitud : 0.0f;
  _player->direction.y = (_player->direction.y != 0.0) ? _player->direction.y / magnitud : 0.0f;
  
  //move
  _player->position.x += _player->direction.x * _player->speed;
  _player->position.y += _player->direction.y * _player->speed;

  _player->hitbox.x = _player->position.x;
  _player->hitbox.y = _player->position.y;
}



//
//PLAYER DRAW
//
void player_draw(player_t* _player){
  if(_player->sprite){
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 255);
    SDL_RenderDrawPointF(g_renderer, _player->position.x,_player->position.y);

    SDL_SetRenderDrawColor(g_renderer, 255, 0, 0 ,255);
    SDL_RenderDrawRectF(g_renderer, &_player->hitbox);

    SDL_RenderCopyF(g_renderer, _player->sprite, NULL, &_player->hitbox);
  }
}

void player_destroy(player_t* _player)
{
  if(_player)
  {
    delete _player;
    _player = nullptr;
  }
  
}
