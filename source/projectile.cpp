#include"projectile.hpp"

/**
 *Allocates memory for a new projectile structure
 *@params _position Initial enemy position on creation
 *@params _direction Initial enemy direction
 *@params _isFriendly Describes if the projectile should damage the player
 *@params _sprite Pointer to an SDL_Texture used for drawing the projectile
 *@return Pointer to an player structure.
 */

projectile_t* projectile_create(SDL_FPoint _position, SDL_FPoint _direction, bool _isFriendly, SDL_Texture* _sprite)
{
  projectile_t* tmp = (projectile_t*)malloc(sizeof(projectile_t));

  tmp->position = _position;
  tmp->direction = _direction;
  tmp->hitbox = {tmp->position.x, tmp->position.y,
                 13.0f,           54.0f};
  tmp->sprite = _sprite;
  tmp->speed = SLOW_PRJ_SPEED;
  tmp->isFriendly = _isFriendly;

  return tmp;
}

/**
 *Updates the projectile position and state.
 *@params _prj The projectile to update.
 *@return In case of deletion return 1.
 */

int projectile_update(projectile_t* _prj){
  if(_prj == NULL)
    return 0;
  
  //on bounds check
  if(_prj->position.x > 600 || _prj->position.x < 0 ||
     _prj->position.y > 600 || _prj->position.y < 0){
    
    return 1;
  }else{
    
    _prj->position.x += _prj->direction.x * _prj->speed;
    _prj->position.y += _prj->direction.y * _prj->speed;

    _prj->hitbox.x = _prj->position.x - (_prj->hitbox.w / 2);
    _prj->hitbox.y = _prj->position.y - (_prj->hitbox.h / 2);

    return 0;
  }
}

/**
 *Draws the projectile on current renderer.
 *@params _prj The projectile to draw.
 *@params _renderer Pointer to SDL_Renderer where the projectile will be drawn.
 */

void projectile_draw(projectile_t* _prj, SDL_Renderer* _renderer)
{
  //drawing projectile
  if(_prj->isFriendly){
    SDL_RenderCopyExF(_renderer,
                      _prj->sprite,
                      NULL,
                      &_prj->hitbox,
                      0.0,
                      NULL,
                      SDL_FLIP_NONE);
  }else{
    SDL_RenderCopyExF(_renderer,
                      _prj->sprite,
                      NULL,
                      &_prj->hitbox,
                      180.0,
                      NULL,
                      SDL_FLIP_NONE);
  }
    //drawing hitbox
  //SDL_FRect smallerHitbox = _prj->hitbox;
  //smallerHitbox.h = _prj->hitbox.h / 3.0f;
  //SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
  //SDL_RenderDrawRectF(gRenderer, &smallerHitbox);
  
}

/**
 *Frees the projectile from memory.
 *@params _prj The projectile to free
 */
void projectile_destroy(projectile_t** _prj)
{
  
  if(_prj || *_prj)
  {
    free(*_prj);
    *_prj = NULL;
  }
}

projectiles_list projectiles_list_create(){

  projectiles_list _pl ={
    .array = NULL,
    .capacity = 0,
    .count = 0
  };

  return _pl;  
}

int projectiles_list_add(projectiles_list* _pl, projectile_t* _prj){
  //check if the pointers are valid  
  if(_pl == NULL || _prj == NULL){
    return RETURN_NULL_POINTER;
  }
  printf("projectiles_list_add\n");
  //if the list is empty 
  if(projectiles_list_is_empty(*_pl)){
    printf("adding first element\n");

    if(projectiles_list_no_capacity(*_pl)){
      printf("allocating space for first element\n");      
      //allocate for first elements
      _pl->array = (projectile_t**)calloc(2, sizeof(projectile_t*));
      _pl->capacity = 2;
      
      //if allocation failed, return
      if(_pl->array == NULL)
        return RETURN_NULL_POINTER;      
    }
    
    _pl->array[0] = _prj;
    _pl->count = 1;
  }else{
    //we need to check if we need more space, in such case we double it
    printf("adding new element\n");
    if(projectiles_list_no_capacity(*_pl)){

      printf("extending capacity\n");
      projectile_t** pl_realloc = (projectile_t**)realloc(_pl->array, sizeof(projectile_t*) * (_pl->capacity * 2));
      
      if(pl_realloc == NULL)
        return RETURN_NULL_POINTER;

      //initialize new spaces
      for(int i = _pl->capacity; i < (_pl->capacity * 2) - 1; i++){
        pl_realloc[i] = NULL;
      }
      
      //update the new memory location and the capacity
      _pl->array = pl_realloc;
      _pl->capacity *= 2;

      //add the new enemy at the end of the array
      _pl->count += 1;
      _pl->array[_pl->count - 1] = _prj;
      printf("added element on extended capacity, position: %d\n", _pl->count - 1);
    }else{
      //if we don't have to realloc, search for an available memory space
      printf("searching free space to add element\n");
      for(int i = 0; i < _pl->capacity; i++){
        if(_pl->array[i] == NULL){
          _pl->array[i] = _prj;
          _pl->count += 1;
          printf("added element on position: %d\n",i);          
          break;
        }
      }
    }
  }

  printf("\n");  
  return RETURN_SUCCESS;
}

int projectiles_list_remove(projectiles_list* _pl, int index){
  //check if index is valid
  if(index < 0 || index > _pl->capacity)
    return RETURN_BAD_INDEX;

  //check if projectile list extist
  if(_pl == NULL || _pl->array == NULL || _pl->array[index] == NULL)
    return RETURN_NULL_POINTER;

  printf("projectiles_list_remove\n");  

  projectile_destroy(&_pl->array[index]);

  _pl->array[index] = NULL;
  _pl->count -= 1;

  printf("projectiles_list_remove\n");  
  return RETURN_SUCCESS;
}

int projectiles_list_clear(projectiles_list* _pl){
  if(_pl == NULL || _pl->array == NULL)
    return RETURN_NULL_POINTER;

  for(int i = 0; i < _pl->capacity; i++){
    if(_pl->array[i] == NULL)
      continue;

    projectiles_list_remove(_pl, i);
  }

  //free(_pl->array);
  //_pl->array = NULL;
  _pl->count = 0;

  return RETURN_SUCCESS;
}

int projectiles_list_rearrenge(projectiles_list* _pl){
  if(_pl == NULL || _pl->array == NULL)
    return RETURN_NULL_POINTER;

  for(int i = 0; i < _pl->capacity; i++){
    if(_pl->array[i] == NULL){
      for(int j = i; j < _pl->capacity; i++){
        if(_pl->array[j] == NULL)
          continue;

        _pl->array[i] == _pl->array[j];
        break;
      }
    }
  }

  return RETURN_SUCCESS;
}

int projectiles_list_is_empty(projectiles_list _pl){
  return (_pl.count <= 0);
}

int projectiles_list_no_capacity(projectiles_list _pl){
  return (_pl.capacity <= _pl.count);
}

int projectiles_list_print(projectiles_list* _pl){
  if(_pl == NULL || _pl->array == NULL)
    return RETURN_NULL_POINTER;

  printf("current count: %d\n", _pl->count);
  printf("current capacity: %ld\n", _pl->capacity);

  for(int i = 0; i < _pl->capacity; i++){
    printf("%d - ", i);
  }
  printf("\n");
  
  for(int i = 0; i < _pl->capacity; i++){
    if(_pl->array[i] == NULL){
      printf("0 - ");
    }else{
      printf("X - ");
    }
  }
  printf("\n");

  return RETURN_SUCCESS;
}
