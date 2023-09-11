#include"enemy.hpp"

/**
 *Allocates memory for an enemy structure.
 *@params _texturesVector An vector of SDL_Textures pointers.
 *@return Pointer to an enemy structure.
 */

enemy_t* enemy_create(std::vector<SDL_Texture*> _texturesVector){
  enemy_t* _tmp = new enemy_t;
  
  _tmp->position = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

    //get texture dimensions
  SDL_Point dimensions = {0};
  SDL_QueryTexture(_texturesVector.at(TXT_ENEMY1_BLACK),
                   NULL,
                   NULL,
                   &dimensions.x,
                   &dimensions.y);

  _tmp->hitbox   = {_tmp->position.x,
                  _tmp->position.y,
                  (float)dimensions.x,
                  (float)dimensions.y};
  _tmp->sprite   = _texturesVector.at(TXT_ENEMY1_BLACK);
  _tmp->projectile_texture   = _texturesVector.at(TXT_LASER_RED);
  _tmp->hp       = 4;
  _tmp->shootDelay = 15;
  _tmp->currentDelay = _tmp->shootDelay;
  _tmp->path_time = 0.0f;
  _tmp->path = NULL;
  
  return _tmp;  
}

/**
 *Updates internal enemy values.
 *@params _projectileList A pointer to list containing all current game projectiles.
 */

void enemy_update(enemy_t* _enemy, std::list<projectile_t*>* _projectileList){

  //shootin'
  if(_enemy->currentDelay <= 0 && _enemy->shootDelay > 0){
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

void enemy_change_path(enemy_t* _enemy, spline* _spline){
  _enemy->path = _spline;
}

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
    delete _enemy;
    _enemy = NULL;
  }
}

/**
 * Updates enemy position based on it's current path and timing.
 * @params _enemy The desired enemy to update.
 */

void enemy_update_path(enemy_t* _enemy){

  if(_enemy->path != NULL){
    
    if(_enemy->path->loop){
      
    }else{
      //if it's not a loop, update position until it's bigger than 1
      if(_enemy->path_time >= 0.0 && _enemy->path_time <= 1.0f){
        SDL_FPoint new_pos = spline_get_point(*_enemy->path, _enemy->path_time);
        _enemy->position = new_pos;
        _enemy->path_time += 0.01;
        
      }

    }
    
  }

  
}

/**
 * Allocates and initializes a pointer to enemies_container structure.
 * @return Pointer to a new enemies_container
 */
enemies_container* enemies_container_create(){
  enemies_container* ec = new enemies_container;

  ec->array = NULL;
  ec->count = 0;
  ec->capacity = 0;
  return ec;
}

/**
 * Adds a new enemy pointer to the container.
 * @params _container Container where the new enemy will be storage.
 * @params _enemy The enemy to storage.
 * @return In case of failure return -1, otherwise returns 0.
 */
int enemies_container_add(enemies_container* _container, enemy_t* _enemy){
  if(_container == NULL || _enemy == NULL){
    return RETURN_NULL_POINTER;
  }
  
  //if the container is empty
  if(enemies_container_is_empty(_container)){
    printf("container is empty..adding first element\n");
    //allocate for the first element
    _container->array = (enemy_t**)malloc(sizeof(enemy_t*));
    
    //if allocation successed the first element is added
    if(_container->array != NULL){
      _container->array[0] = _enemy;
      _container->count = 1;
      _container->capacity = 1;
    }
  }else{
    printf("container is not empty..adding new element at end\n");    
    //we need to check if we need more space, in such case we double it
    if(_container->count == _container->capacity){

      enemy_t** ec_realloc = (enemy_t**)realloc(_container->array, sizeof(enemy_t*) * (_container->capacity * 2));

      //if realloc failes we exit
      if(ec_realloc == NULL){
        return RETURN_NULL_POINTER;
      }

      //update the new memory location and the capacity
      _container->array = ec_realloc;
      _container->capacity *= 2;
    }
    
    //add the new enemy at the end of the array
    _container->array[_container->count + 1] = _enemy;
    _container->count += 1;
  }

  //if we don't have to realloc, search for an available memory space
  for(int i = 0; i < _container->capacity; i++){

    if(_container->array[i] == NULL){
      _container->array[i] == _enemy;
      break;
    }
    
  }

  return RETURN_SUCCESS;
}

/**
 * Removes an enemy pointer from the container.
 * @params _container Container from where the enemy will be removed.
 * @params _index The index of the enemy.
 * @return In case of failure return -1, otherwise returns 0.
 */
int enemies_container_remove(enemies_container* _container, int _index){

  if(_index < 0 || _index > _container->capacity)
    return RETURN_BAD_INDEX;

  if(_container == NULL || _container->array[_index] == NULL)
    return RETURN_NULL_POINTER;

    free(_container->array[_index]);
    _container->array[_index] = NULL;
    _container->count -= 1;

    return RETURN_SUCCESS;
}

/**
 * Arrenges the array and fits the total capacity to the count
 * @params _container Container from where the enemy will be removed
 * @params _index The index of the enemy
 * @return In case of failure return -1, otherwise returns 0
 */

int enemies_container_rearrenge(enemies_container* _container){
  if(_container == NULL)
    return RETURN_NULL_POINTER;

  //if we have unused space and the unused space is more than the 50% of the capacity
  if(_container->count < _container->capacity && _container->count < _container->capacity / 2){

    //put everything contiguosly
    for(int i = 0; i < _container->capacity; i++){
      if(_container->array[i] == NULL){
        for(int j = i + 1; j < _container->capacity; j++){
          if(_container->array[j] != NULL){
            _container->array[i] == _container->array[j];
            break;
          }
        }
      }
    }

    //realloc to the correct size
    enemy_t** ec_realloc = (enemy_t**)realloc(_container->array, _container->count);

    if(ec_realloc == NULL)
      return RETURN_NULL_POINTER;

    _container->array = ec_realloc;
    _container->capacity = _container->count;
  }

  return RETURN_SUCCESS;
}

/**
 * Clears the internal array of the container.
 * @params _container Container to clean.
 * @return Returns -1 in case of failure, otherwise return 0.
 */
int enemies_container_clear(enemies_container* _container){
  if(_container == NULL || _container->array == NULL)
    return RETURN_NULL_POINTER;

  for(int i = 0; i < _container->capacity; i++){
    if(_container->array == NULL)
      continue;

    enemies_container_remove(_container, i);
  }

  free(_container->array);
  _container->array = NULL;
  
  return RETURN_SUCCESS;
}


/**
 * Destroys the container structure. This makes a call to enemies_container_clear
 * so it's not need to call it before it.
 * @params _container Double pointer to the container to destroy.
 * @return Returns -1 in case of failure, otherwise return 0.
 */
int enemies_container_destroy(enemies_container** _container){
  if(_container == NULL || *_container == NULL)
    return RETURN_NULL_POINTER;

  enemies_container* ec = *_container;
  if(ec->array != NULL)
    enemies_container_clear(ec);

  free(ec);
  ec = NULL;

  return RETURN_SUCCESS;
}

/**
 * Looks if the container doesn't have elements
 * @params _container The container to check
 * @return Returns true in case is empty, 0 otherwise.
 */
bool enemies_container_is_empty(enemies_container* _container){
  return (_container->count <= 0);
}

/**
 */
void enemies_container_print(enemies_container* _container){
  printf("current count: %d\n", _container->count);
  printf("current capacity: %d\n", _container->capacity);

  for(int i = 0; i < _container->capacity; i++){
    if(_container->array[i] == NULL){
      printf("0 - ");
    }else{
      printf("X - ");
    }
  }
  printf("\n");
}
