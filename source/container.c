#include"container.c"


/**
 * Allocates and initializes a pointer to enemies_container structure.
 * @return Pointer to a new enemies_container
 */
enemies_container* enemies_container_create(){
  enemies_container* ec = (enemies_container*)malloc(sizeof(enemies_container));

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

    
      //add the new enemy at the end of the array
      _container->array[_container->count + 1] = _enemy;
      _container->count += 1;
    }else{
      //if we don't have to realloc, search for an available memory space
      for(int i = 0; i < _container->capacity; i++){
        if(_container->array[i] == NULL){
          _container->array[i] == _enemy;
          break;
        }
      }
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

