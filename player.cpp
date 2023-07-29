#include"player.hpp"



//
//PLAYER CREATION
//
player_t* player_create(){
  player_t* tmp_player = new player_t();

  SDL_Point dims = {0, 0};
  SDL_QueryTexture(gTextures.at(TXT_PLAYER_BLUE), NULL, NULL,
                   &dims.x,
                   &dims.y);
  
  tmp_player->hp        = 5;
  tmp_player->position  = {300.0f, 600.0f - dims.y};
  tmp_player->direction = {0};
  tmp_player->hitbox    = {tmp_player->position.x,
                           tmp_player->position.y,
                           (float)dims.x,
                           (float)dims.y};
  tmp_player->shootDelay = 15;
  tmp_player->currentShootDelay = 15;
  tmp_player->speed     = 6.0f;
  tmp_player->sprite    = gTextures.at(TXT_PLAYER_BLUE);

  return tmp_player;
}



//
//PLAYER LOGIC UPDATE
//
void player_update(player_t* _player, const Uint8* _keyboardState){
  //get inputs

  //move vertically
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

  //move horizontally
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

  //shoot projectile
  if(_keyboardState[SDL_SCANCODE_J] && _player->currentShootDelay < 0)
  {
    projectile_t* tmp_prj = projectile_create(_player->position, {0.0f, -1.0f});      
    gProjectiles.push_back(tmp_prj);

    _player->currentShootDelay = _player->shootDelay;
  }
  else
  {
    _player->currentShootDelay--;
  }


  //normalizing vector
  float magnitud = sqrt(_player->direction.x * _player->direction.x + _player->direction.y * _player->direction.y);
  _player->direction.x = (_player->direction.x != 0.0) ? _player->direction.x / magnitud : 0.0f;
  _player->direction.y = (_player->direction.y != 0.0) ? _player->direction.y / magnitud : 0.0f;
  
  //move
  _player->position.x += _player->direction.x * _player->speed;
  _player->position.y += _player->direction.y * _player->speed;

  _player->hitbox.x = _player->position.x - (_player->hitbox.w / 2);
  _player->hitbox.y = _player->position.y - (_player->hitbox.h / 2);
}



//
//PLAYER DRAW
//
void player_draw(player_t* _player){
  if(_player->sprite){

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
    SDL_RenderDrawPointF(gRenderer, _player->position.x,_player->position.y);

    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0 ,255);
    SDL_RenderDrawRectF(gRenderer, &_player->hitbox);

    SDL_RenderCopyF(gRenderer, _player->sprite, NULL, &_player->hitbox);
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
