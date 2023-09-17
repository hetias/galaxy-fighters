#include "scene.hpp"

/**
 * Allocates memory for a new scene structure
 * @param _texture_paths
 * @return pointer to a new scene_t
 */
scene_t* scene_create(const char** _texture_paths){
  scene_t* new_scene = new scene_t;

  //Load resources
  scene_load_resources(new_scene, _texture_paths);

  //Create player
  new_scene->main_player = player_create(&new_scene->textures_vector);

  //Clean lists
  new_scene->projectiles = projectiles_list_create();

  //create enemy container
  new_scene->enemies_array = enemies_container_create();

  //start ticks
  new_scene->tick = 0;
  
  return new_scene;
}

/**
 * Load game resources
 * @param _scene scene where the structures containing resources are present
 * @param _textures_path Path to game textures
 */
void scene_load_resources(scene_t* _scene, const char** _textures_paths){
  SDL_Texture* _tmpTexture = nullptr;

  //OPEN GAME FONT
  // gFont = TTF_OpenFont("../resources/fonts/font.ttf", 24);
  // if (!gFont){
  //   std::cout<<SDL_GetError()<<std::endl;
  //   return EXIT_FAILURE;
  // }

  //
  //LOOAD TEXTURES
  //

  SDL_Texture* _tmp = nullptr;
  for(int i = 0; i < TXT_VECSIZE; i++){
    _tmp = IMG_LoadTexture(gRenderer, _textures_paths[i]);
    
    _scene->textures_vector.push_back(_tmp);
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

  player_update(_scene->main_player, keyboardState, &_scene->projectiles);
  scene_update_projectiles(&_scene->projectiles);
  //scene_update_enemies(&_scene->enemies_array);
  
  _scene->tick++;

  return RETURN_SUCCESS;
}

/**
 * Update all projectiles in a scene
 * @param _scene Scene containing a list of all projectiles
 * @return In case of succes returns RETURN_SUCCESS, in case pointers parameters are invalid returns RETURN_NULL_POINTER
 */
int scene_update_projectiles(projectiles_list* _projectilesList){
  if(_projectilesList == NULL || _projectilesList->array == NULL)
    return RETURN_NULL_POINTER;

  projectiles_list_print(_projectilesList);
  
  for(int i = 0; i < _projectilesList->capacity; i++){
    if(_projectilesList->array[i] == NULL){
      printf("Null element...continue\n");
    }else{

      if(projectile_update(_projectilesList->array[i])){
        printf("Element to deletion..\n");
        projectiles_list_remove(_projectilesList, i);
      }
    }
  }

  //projectiles_list_print(_projectilesList);  
  return RETURN_SUCCESS;
}


/**
 * Update all enemies on the current scene
 * @param enemies_container A pointer to the enemies_container structure
where the enemies are located
* @return Returns RETURN_SUCCESS on succes, in case there are invalid pointers it returns RETURN_NULL_POINTER
 */
int scene_update_enemies(enemies_container** _enemies_container){
  if(_enemies_container == NULL || *_enemies_container == NULL)
  {

    return RETURN_NULL_POINTER;
  }
  enemies_container* ec = *_enemies_container;
  
  //printf("there are currently %d enemies\n", ec->count);

  return RETURN_SUCCESS;
}



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
  SDL_RenderCopy(gRenderer, _scene->textures_vector.at(TXT_BG_DARKPURPLE), NULL, NULL);

  //draw the player
  player_draw(_scene->main_player, _renderer);

  //draw the enemies
  if(scene_draw_enemies(_scene->enemies_array, _renderer) < 0){
    printf("Null reference on scene_draw_enemies\n");
  }

  //draw the projectiles
  scene_draw_projectiles(_scene->projectiles, _renderer);

  return RETURN_SUCCESS;
}



/**
 * Draw on screen all present projectiles on a scene
 * @param _scene Scene with a structure where all projectiles are stored
 * @param _renderer SDL2 renderer where everything will be drawn
* @return Returns RETURN_SUCCESS on succes, in case there are invalid pointers it returns RETURN_NULL_POINTER
 */
int scene_draw_projectiles(projectiles_list _projectilesList, SDL_Renderer* _renderer){
  if(_renderer == NULL)
    return RETURN_NULL_POINTER;

  projectile_t** _projectiles_array = _projectilesList.array;

  for(int i = 0; i < _projectilesList.capacity; i++){
    if(_projectiles_array[i] == NULL)
      continue;

    projectile_draw(_projectiles_array[i], _renderer);
  }

  return RETURN_SUCCESS;
}


/**
 * Draws all enemies on current scene
 * @param _enemies_container A container structure where all enemies are located
 * @param _renderer A SDL2 Renderer where everything will be drawn
 * @return Returns RETURN_SUCCESS on succes, in case there are invalid pointers it returns RETURN_NULL_POINTER
 */
int scene_draw_enemies(enemies_container* _enemies_container, SDL_Renderer* _renderer){
  if(_enemies_container == NULL || _renderer == NULL){
    return RETURN_NULL_POINTER; 
  }
  
  for(int i = 0; i < _enemies_container->capacity; i++){
    if(_enemies_container->array[i] == NULL)
      continue;

    enemy_draw(_enemies_container->array[i], _renderer);
  }

  return RETURN_SUCCESS;
}


/**
 * Destroy scene and free it's space
 * @param _scene Scene to free
 * @return In case of succes returns RETURN_SUCCESS, in case pointers parameters are invalid returns RETURN_NULL_POINTER
 */
int scene_destroy(scene_t* _scene){
  if(_scene == NULL)
    return RETURN_NULL_POINTER;
  
  //destroy player
  player_destroy(_scene->main_player);
  
  //destroy all enemies
  enemies_container_destroy(&_scene->enemies_array);
  
  //destroy all projectiles
  projectiles_list_clear(&_scene->projectiles);
  
  //free resources
  for(auto texture : _scene->textures_vector){
    SDL_DestroyTexture(texture);
  }
  _scene->textures_vector.clear();

  return RETURN_SUCCESS;
}
