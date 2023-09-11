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
  new_scene->projectile_list.clear();

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
 */
void scene_update(scene_t* _scene){

  //input
  const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

  if(_scene->tick == 300){    
    enemy_t* enemy1 = enemy_create(_scene->textures_vector);
    enemies_container_add(_scene->enemies_array, enemy1);

    enemy_t* enemy2 = enemy_create(_scene->textures_vector);
    enemies_container_add(_scene->enemies_array, enemy2);

    enemy_t* enemy3 = enemy_create(_scene->textures_vector);
    enemies_container_add(_scene->enemies_array, enemy3);
        
    enemies_container_print(_scene->enemies_array);
  }

  if(_scene->tick == 800){
    (enemies_container_remove(_scene->enemies_array, 2));
    enemies_container_print(_scene->enemies_array); 
  }
  
  player_update(_scene->main_player, keyboardState, &_scene->projectile_list);
  scene_update_projectiles(&_scene->projectile_list);
  scene_update_enemies(&_scene->enemies_array);
  
  _scene->tick++;
}

/**
 * Update all projectiles in a scene
 * @param _scene Scene containing a list of all projectiles
 */
void scene_update_projectiles(std::list<projectile_t*> *_projectileList){
  for(std::list<projectile_t*>::iterator _prj = _projectileList->begin();
      _prj != _projectileList->end();
      _prj++){
    
    if(projectile_update(*_prj)){
      printf("projectile update, size: %ld\n", _projectileList->size());
      if(_projectileList->size() > 1){
        printf("destroy \n");
        projectile_destroy(*_prj);
        printf("clear \n");
        _projectileList->erase(_prj);
      }else{
        printf("destroy \n");
        projectile_destroy(*_prj);

        printf("clear \n");
        _projectileList->clear();
        break;
      }
    }
  }
  
}

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
 */
void scene_draw(scene_t* _scene, SDL_Renderer* _renderer){  
  /*background elements*/
  SDL_RenderCopy(gRenderer, _scene->textures_vector.at(TXT_BG_DARKPURPLE), NULL, NULL);

  //draw the player
  player_draw(_scene->main_player, _renderer);

  //draw the enemies
  if(scene_draw_enemies(_scene->enemies_array, _renderer) < 0){
    printf("Null reference on scene_draw_enemies\n");
  }

  //draw the projectiles
  scene_draw_projectiles(_scene, _renderer);
}



/**
 * Draw on screen all present projectiles on a scene
 * @param _scene Scene with a structure where all projectiles are stored
 * @param _renderer SDL2 renderer where everything will be drawn
 */
void scene_draw_projectiles(scene_t* _scene, SDL_Renderer* _renderer){

  for(auto _prj : _scene->projectile_list){
    projectile_draw(_prj, _renderer);
  }

}

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
 */
void scene_destroy(scene_t* _scene){
  
  //destroy player
  player_destroy(_scene->main_player);
  
  //destroy all enemies
  enemies_container_destroy(&_scene->enemies_array);
  
  //destroy all projectiles
  for(auto prj : _scene->projectile_list){
    projectile_destroy(prj);
  }
  _scene->projectile_list.clear();

  //free resources
  for(auto texture : _scene->textures_vector){
    SDL_DestroyTexture(texture);
  }
  _scene->textures_vector.clear();
  
}
