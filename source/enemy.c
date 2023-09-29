#include"enemy.h"

/**
 *Allocates memory for an enemy structure.
 *@params _texturesVector An vector of SDL_Textures pointers.
 *@return Pointer to an enemy structure.
 */

enemy_t* enemy_create(SDL_Texture** _texturesVector){
  enemy_t* _tmp = (enemy_t*)malloc(sizeof(enemy_t));
  
  
  _tmp->position = (SDL_FPoint){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

    //get texture dimensions
  SDL_Point dimensions = (SDL_Point){0};
  SDL_QueryTexture(_texturesVector[TXT_ENEMY1_BLACK],
                   NULL,
                   NULL,
                   &dimensions.x,
                   &dimensions.y);

  _tmp->hitbox   = (SDL_FRect){_tmp->position.x,
                  _tmp->position.y,
                  (float)dimensions.x,
                  (float)dimensions.y};
  _tmp->sprite   = _texturesVector[TXT_ENEMY1_BLACK];
  _tmp->projectile_texture   = _texturesVector[TXT_LASER_RED];
  _tmp->hp       = 4;
  _tmp->shootDelay = 15;
  _tmp->currentDelay = _tmp->shootDelay;
  _tmp->path_time = 0.0f;
  //_tmp->path = NULL;
  
  return _tmp;  
}

/**
 *Updates internal enemy values.
 *@params _projectileList A pointer to list containing all current game projectiles.
 */

//std::list<projectile_t*>* _projectileList
void enemy_update(enemy_t* _enemy){

  //shootin'
  if(_enemy->currentDelay <= 0 && _enemy->shootDelay > 0){
    //projectile_t* tmp_prj = projectile_create(_enemy->position,{0.0, 1.0},false,_enemy->projectile_texture);
  //_projectileList->push_back(tmp_prj);

    _enemy->currentDelay = _enemy->shootDelay;
  }else{
    _enemy->currentDelay--;
  }

  //movin'
  enemy_update_path(_enemy);
  
  //hitbox repositioning
  _enemy->hitbox.x = _enemy->position.x - _enemy->hitbox.w / 2;
  _enemy->hitbox.y = _enemy->position.y - _enemy->hitbox.h / 2;
}

/**
 *Updates enemy position based on it's current path and timing.
 *@params _enemy Pointer to the desired enemy to modify.
 *@params _spline Pointer to the new spline to assign to enemy.
 */

/* void enemy_change_path(enemy_t* _enemy, spline* _spline){ */
/*   _enemy->path = _spline; */
/* } */

/**
 *Draws the enemy texture to the game's renderer
 *@params _enemy Pointer to enemy to draw
 *@params _renderer Pointer to an SDL_Renderer where the player is drawn
 */

void enemy_draw(enemy_t* _enemy, SDL_Renderer* _renderer){  
  if(_enemy){

    SDL_RenderCopyF(_renderer,
                   _enemy->sprite,
                   NULL,
                   &_enemy->hitbox
                   );
  }
  /*
    DEBUG DRAWING
   
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

/**
 * Free's enemy resources from memory.
 * @params _enemy Pointer to enemy to be freed.
 */

void enemy_destroy(enemy_t* _enemy){
  if(_enemy)
  {
    _enemy->sprite = NULL;
    _enemy->projectile_texture = NULL;
    
    free(_enemy);
    _enemy = NULL;
  }
}

/**
 * Updates enemy position based on it's current path and timing.
 * @params _enemy The desired enemy to update.
 */

void enemy_update_path(enemy_t* _enemy){

  /* if(_enemy->path != NULL){ */
    
  /*   if(_enemy->path->loop){ */
      
  /*   }else{ */
  /*     //if it's not a loop, update position until it's bigger than 1 */
  /*     if(_enemy->path_time >= 0.0 && _enemy->path_time <= 1.0f){ */
  /*       SDL_FPoint new_pos = spline_get_point(*_enemy->path, _enemy->path_time); */
  /*       _enemy->position = new_pos; */
  /*       _enemy->path_time += 0.01; */
        
  /*     } */

  /*   } */
    
  /* } */

  
}
