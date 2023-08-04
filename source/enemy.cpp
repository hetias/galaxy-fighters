#include"enemy.hpp"

enemy_t* enemy_create()
{
  enemy_t* _tmp = new enemy_t;
  
  _tmp->position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

    //get texture dimensions
  SDL_Point dimensions = {0};
  SDL_QueryTexture(gTextures.at(TXT_ENEMY1_BLACK),
                   NULL,
                   NULL,
                   &dimensions.x,
                   &dimensions.y);

  _tmp->hitbox   = {_tmp->position.x,
                  _tmp->position.y,
                  (float)dimensions.x,
                  (float)dimensions.y};
  _tmp->sprite   = gTextures.at(TXT_ENEMY1_BLACK);
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

void enemy_destroy(enemy_t* _enemy)
{
  if(_enemy)
  {
    delete _enemy;
    _enemy = NULL;
  }
}