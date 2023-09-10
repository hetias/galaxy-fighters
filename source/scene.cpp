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
  new_scene->enemy_vector.clear();
  
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
  
  player_update(_scene->main_player, keyboardState, &_scene->projectile_list);
  scene_update_projectiles(&_scene->projectile_list);
  scene_update_enemies(_scene);
  
  _scene->tick++;
}

/**
 * Update all projectiles in a scene
 * @param _scene Scene containing a list of all projectiles
 */
void scene_update_projectiles(std::list<projectile_t*> *_projectileList){
  printf("scene_update\n");
  
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

void scene_update_enemies(scene_t* _scene){
  
  for(auto _enemy : _scene->enemy_vector){
    enemy_update(_enemy, &_scene->projectile_list);
  }
  
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

void scene_draw_enemies(scene_t* _scene, SDL_Renderer* _renderer){

  for(auto _enemy : _scene->enemy_vector){
    enemy_draw(_enemy, _renderer);
  }
  
}

/**
 * Draw all scene entities and details
 * @param _scene 
 * @param _renderer
 */
void scene_draw(scene_t* _scene, SDL_Renderer* _renderer){  
  /*background elements*/
  SDL_RenderCopy(gRenderer, _scene->textures_vector.at(TXT_BG_DARKPURPLE), NULL, NULL);

  player_draw(_scene->main_player, _renderer);
  scene_draw_enemies(_scene, _renderer);
  scene_draw_projectiles(_scene, _renderer);
}


/**
 * Destroy scene and free it's space
 * @param _scene Scene to free
 */
void scene_destroy(scene_t* _scene){
  
  //destroy player
  player_destroy(_scene->main_player);
  
  //destroy all enemies
  for(auto enemy : _scene->enemy_vector){
    enemy_destroy(enemy);
  }
  _scene->enemy_vector.clear();

  //destroy all enemies
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
