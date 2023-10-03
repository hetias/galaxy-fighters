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

  //create the player
  new_scene->player = player_create(new_scene->textures_vector);

  //create one enemy, just 4 now
  new_scene->enemy = enemy_create(new_scene->textures_vector);

  //create container for projectiles
  new_scene->projectiles_container = container_create(CONTAINER_PROJECTILE);
    
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
 * @return In case of succes returns 0, in case pointers parameters are invalid returns -1
 */
int scene_update(scene_t* _scene){

  //input
  const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

  player_update(_scene->player, keyboardState, &_scene->projectiles_container);
  enemy_update(_scene->enemy);
  
  //update projectiles
  scene_update_projectiles(&_scene->projectiles_container);
  
  _scene->tick++;

  return 0;
}

/**
 * Update all projectiles in a scene
 * @param _scene Scene containing a list of all projectiles
 * @return In case of succes returns 0, in case pointers parameters are invalid returns -1
 */
int scene_update_projectiles(game_container* projectiles_container){
  if(projectiles_container == NULL || projectiles_container->array == NULL)
    return -1;

  for(int i = 0; i < projectiles_container->count; i++){

    if( projectile_update(projectiles_container->array[i]) ){
      container_remove(projectiles_container, i);
    }
    
  }

  
  return 0;
}


/**
 * Update all enemies on the current scene
 * @param enemies_container A pointer to the enemies_container structure
where the enemies are located
* @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
/* int scene_update_enemies(enemies_container** _enemies_container){ */
/*   if(_enemies_container == NULL || *_enemies_container == NULL) */
/*   { */

/*     return -1; */
/*   } */
/*   enemies_container* ec = *_enemies_container; */
  
/*   //printf("there are currently %d enemies\n", ec->count); */

/*   return 0; */
/* } */



/**
 * Draw all scene entities and background
 * @param _scene 
 * @param _renderer
* @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
int scene_draw(scene_t* _scene, SDL_Renderer* _renderer){  
  if(_scene == NULL || _renderer == NULL)
    return -1;
  
  /*background elements*/
  SDL_RenderCopy(_renderer, _scene->textures_vector[TXT_BG_DARKPURPLE], NULL, NULL);

  //draw the player
  player_draw(_scene->player, _renderer);

  //draw enemies
  //enemy_draw(_scene->enemy, _renderer);

  //draw projectiles
  for(int i = 0; i < _scene->projectiles_container.count; i++){
    projectile_draw(_scene->projectiles_container.array[i], _renderer);
  }


  return 0;
}



/**
 * Draw on screen all present projectiles on a scene
 * @param _scene Scene with a structure where all projectiles are stored
 * @param _renderer SDL2 renderer where everything will be drawn
* @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
int scene_draw_projectiles(game_container* projectiles_container, SDL_Renderer* _renderer){
  if(_renderer == NULL)
    return -1;

  for(int i = 0; i < projectiles_container->count; i++){
    if(projectiles_container->array[i] == NULL)
      continue;

    projectile_draw(projectiles_container->array[i], _renderer);
  }

  return 0;
}


/**
 * Draws all enemies on current scene
 * @param _enemies_container A container structure where all enemies are located
 * @param _renderer A SDL2 Renderer where everything will be drawn
 * @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
/* int scene_draw_enemies(enemies_container* _enemies_container, SDL_Renderer* _renderer){ */
/*   if(_enemies_container == NULL || _renderer == NULL){ */
/*     return -1;  */
/*   } */
  
/*   for(int i = 0; i < _enemies_container->capacity; i++){ */
/*     if(_enemies_container->array[i] == NULL) */
/*       continue; */

/*     enemy_draw(_enemies_container->array[i], _renderer); */
/*   } */

/*   return 0; */
/* } */


/**
 * Destroy scene and free it's space
 * @param _scene Scene to free
 * @return In case of succes returns 0, in case pointers parameters are invalid returns -1
 */
int scene_destroy(scene_t* _scene){
  if(_scene == NULL)
    return -1;

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
  
  return 0;
}
