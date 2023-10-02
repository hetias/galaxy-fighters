#include "scene.h"

/**
 * Allocates memory for a new scene structure
 * @param _texture_paths
 * @return pointer to a new scene_t
 */
scene_t* scene_create(const char** _texture_paths, SDL_Renderer* _renderer){
  scene_t* new_scene = (scene_t*)malloc(sizeof(scene_t));

  //Load resources
  scene_load_resources(new_scene, _texture_paths, _renderer);

  new_scene->player = player_create(new_scene->textures_vector);

  new_scene->enemy = enemy_create(new_scene->textures_vector);

  new_scene->projectiles_container = container_create(32, CONTAINER_PROJECTILE);

  printf("Adding elements on the container\n");
  for(int i = 0; i < 32; i++){
    projectile_t* proj = (projectile_t*)malloc(sizeof(projectile_t));
    proj->position = (SDL_FPoint){i * 1, i * 1};
    proj->direction = (SDL_FPoint){0, 1};
    proj->hitbox = (SDL_FRect){20, 20};
    proj->sprite = new_scene->textures_vector[TXT_LASER_RED];
    proj->speed = 5.0f;
    proj->isFriendly = false;

    container_add(&new_scene->projectiles_container, (void*)proj);
  }

  /* printf("Container post additions\n"); */
  /* container_print(new_scene->projectiles_container); */
  
  /* getchar(); */
  
  /* printf("Container after additions\n"); */
  /* container_print(new_scene->projectiles_container); */
  /* for(int i = 0; i < new_scene->projectiles_container.count; i++){     */
  /*   printf("Iteration number: %d\n", i); */
  /*   if(i % 2 == 0) container_remove(&new_scene->projectiles_container, i); */
  /*   container_print(new_scene->projectiles_container); */
  /* } */

  /* printf("Container post additions\n"); */
  /* container_print(new_scene->projectiles_container); */
  
  //getchar();
    
  //start ticks
  new_scene->tick = 0;
  
  return new_scene;
}

/**
 * Load game resources
 * @param _scene scene where the structures containing resources are present
 * @param _textures_path Path to game textures
 */
void scene_load_resources(scene_t* _scene, const char** _textures_paths, SDL_Renderer* _renderer){

  //load all resources
  for(int i = 0; i < TXT_TOTAL; i++){

    _scene->textures_vector[i] = NULL;
    _scene->textures_vector[i] = IMG_LoadTexture(_renderer, _textures_paths[i]);

    if(_scene->textures_vector[i] == NULL)
      printf("Warning: failed to load: %s\n", _textures_paths[i]);
  }

}

/**
 * Update scenario and all it's current entities 
 * @param _scene Scene to update
 * @return In case of succes returns RETURN_SUCCESS, in case pointers parameters are invalid returns RETURN_NULL_POINTER
 */
int scene_update(scene_t* _scene){

  //input
  const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

  // player_update(_scene->main_player, keyboardState, &_scene->projectiles);

  player_update(_scene->player, keyboardState);
  enemy_update(_scene->enemy);

  _scene->tick++;

  return RETURN_SUCCESS;
}

/**
 * Update all projectiles in a scene
 * @param _scene Scene containing a list of all projectiles
 * @return In case of succes returns RETURN_SUCCESS, in case pointers parameters are invalid returns RETURN_NULL_POINTER
 */
/* int scene_update_projectiles(projectiles_list* _projectilesList){ */
/*   if(_projectilesList == NULL || _projectilesList->array == NULL) */
/*     return RETURN_NULL_POINTER; */

/*   projectiles_list_print(_projectilesList); */
  
/*   for(int i = 0; i < _projectilesList->capacity; i++){ */
/*     if(_projectilesList->array[i] == NULL){ */
/*       //printf("Null element...continue\n"); */
/*     }else{ */

/*       if(projectile_update(_projectilesList->array[i])){ */
/*         printf("Element to deletion..\n"); */
/*         projectiles_list_remove(_projectilesList, i); */
/*       } */
/*     } */
/*   } */

/*   //projectiles_list_print(_projectilesList);   */
/*   return RETURN_SUCCESS; */
/* } */


/**
 * Update all enemies on the current scene
 * @param enemies_container A pointer to the enemies_container structure
where the enemies are located
* @return Returns RETURN_SUCCESS on succes, in case there are invalid pointers it returns RETURN_NULL_POINTER
 */
/* int scene_update_enemies(enemies_container** _enemies_container){ */
/*   if(_enemies_container == NULL || *_enemies_container == NULL) */
/*   { */

/*     return RETURN_NULL_POINTER; */
/*   } */
/*   enemies_container* ec = *_enemies_container; */
  
/*   //printf("there are currently %d enemies\n", ec->count); */

/*   return RETURN_SUCCESS; */
/* } */



/**
 * Draw all scene entities and background
 * @param _scene 
 * @param _renderer
* @return Returns RETURN_SUCCESS on succes, in case there are invalid pointers it returns RETURN_NULL_POINTER
 */
int scene_draw(scene_t* _scene, SDL_Renderer* _renderer){  
  if(_scene == NULL || _renderer == NULL)
    return RETURN_NULL_POINTER;
  
  /*background elements*/
  SDL_RenderCopy(_renderer, _scene->textures_vector[TXT_BG_DARKPURPLE], NULL, NULL);

  //draw the player
  player_draw(_scene->player, _renderer);

  //draw enemies
  enemy_draw(_scene->enemy, _renderer);

  return RETURN_SUCCESS;
}



/**
 * Draw on screen all present projectiles on a scene
 * @param _scene Scene with a structure where all projectiles are stored
 * @param _renderer SDL2 renderer where everything will be drawn
* @return Returns RETURN_SUCCESS on succes, in case there are invalid pointers it returns RETURN_NULL_POINTER
 */
/* int scene_draw_projectiles(projectiles_list _projectilesList, SDL_Renderer* _renderer){ */
/*   if(_renderer == NULL) */
/*     return RETURN_NULL_POINTER; */

/*   projectile_t** _projectiles_array = _projectilesList.array; */

/*   for(int i = 0; i < _projectilesList.capacity; i++){ */
/*     if(_projectiles_array[i] == NULL) */
/*       continue; */

/*     projectile_draw(_projectiles_array[i], _renderer); */
/*   } */

/*   return RETURN_SUCCESS; */
/* } */


/**
 * Draws all enemies on current scene
 * @param _enemies_container A container structure where all enemies are located
 * @param _renderer A SDL2 Renderer where everything will be drawn
 * @return Returns RETURN_SUCCESS on succes, in case there are invalid pointers it returns RETURN_NULL_POINTER
 */
/* int scene_draw_enemies(enemies_container* _enemies_container, SDL_Renderer* _renderer){ */
/*   if(_enemies_container == NULL || _renderer == NULL){ */
/*     return RETURN_NULL_POINTER;  */
/*   } */
  
/*   for(int i = 0; i < _enemies_container->capacity; i++){ */
/*     if(_enemies_container->array[i] == NULL) */
/*       continue; */

/*     enemy_draw(_enemies_container->array[i], _renderer); */
/*   } */

/*   return RETURN_SUCCESS; */
/* } */


/**
 * Destroy scene and free it's space
 * @param _scene Scene to free
 * @return In case of succes returns RETURN_SUCCESS, in case pointers parameters are invalid returns RETURN_NULL_POINTER
 */
int scene_destroy(scene_t* _scene){
  if(_scene == NULL)
    return RETURN_NULL_POINTER;

  //free resources
  for(int i = 0; i < TXT_TOTAL; i++){

    if(_scene->textures_vector[i] != NULL){
      SDL_DestroyTexture(_scene->textures_vector[i]);
      _scene->textures_vector[i] = NULL;
    }
    
  }

  //destroy container
  container_clear(&_scene->projectiles_container);
  
  //destroy player
  player_destroy(_scene->player);

  //destroy enemies
  enemy_destroy(&_scene->enemy);
  
  free(_scene);
  
  return RETURN_SUCCESS;
}
