#include"enemy.hpp"

enemy_t* enemy_create(std::vector<SDL_Texture*>* _texturesVector)
{
  if(!_texturesVector){
    std::cout<<""<<std::endl;
  }
  
  enemy_t* _tmp = new enemy_t;
  
  _tmp->position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

    //get texture dimensions
  SDL_Point dimensions = {0};
  SDL_QueryTexture(_texturesVector->at(TXT_ENEMY1_BLACK),
                   NULL,
                   NULL,
                   &dimensions.x,
                   &dimensions.y);

  _tmp->hitbox   = {_tmp->position.x,
                  _tmp->position.y,
                  (float)dimensions.x,
                  (float)dimensions.y};
  _tmp->sprite   = _texturesVector->at(TXT_ENEMY1_BLACK);
  _tmp->projectile_texture   = _texturesVector->at(TXT_LASER_RED);
  _tmp->hp       = 4;
  _tmp->shootDelay = 15;
  _tmp->currentDelay = _tmp->shootDelay;
  _tmp->path_time = 0.0f;
  _tmp->path = NULL;
  
  return _tmp;  
}

void enemy_update(enemy_t* _enemy, std::list<projectile_t*>* _projectileList)
{

  //shootin'
  if(_enemy->currentDelay < 0){
    projectile_t* tmp_prj = projectile_create(_enemy->position,
                                              {0.0, 1.0},
                                              false,
                                              _enemy->projectile_texture);
    _projectileList->push_back(tmp_prj);

    _enemy->currentDelay = _enemy->shootDelay;
  }else{
    _enemy->currentDelay--;
  }

  //movin'
  enemy_update_path(_enemy, _enemy->path);
  
  //hitbox repositioning
  _enemy->hitbox.x = _enemy->position.x - _enemy->hitbox.w / 2;
  _enemy->hitbox.y = _enemy->position.y - _enemy->hitbox.h / 2;
}

void enemy_change_path(enemy_t* _enemy, spline* _spline){
  _enemy->path = _spline;
}

void enemy_draw(enemy_t* _enemy, SDL_Renderer* _renderer)
{  
  if(_enemy){

    SDL_RenderCopyF(_renderer,
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

void enemy_update_path(enemy_t* _enemy, spline* _spline){

  if(_enemy->path != NULL){
    
    if(_enemy->path->loop){
      
    }else{
      //if it's not a loop, update position until it's bigger than 1
      if(_enemy->path_time <= 1.0f){
        SDL_FPoint new_pos = spline_get_point(*_spline, _enemy->path_time);
        _enemy->position = new_pos;
        _enemy->path_time += 0.01;
        
      }

    }
    
  }

  
}
