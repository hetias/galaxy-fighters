#include "../include/scene.h"
#include <SDL2/SDL_render.h>
#include<string.h>

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

    //create container for projectiles
    new_scene->projectiles_container = container_create(CONTAINER_PROJECTILE);
    
    //create enemy container
    new_scene->enemies_container = container_create(CONTAINER_ENEMY);
    new_scene->max_enemy_id = 0;
    
    //scene actions    
    scene_load_level("level.lvl", new_scene);
    printf("loaded %d paths from file\n", new_scene->spline_count);
     
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
    if(_scene == NULL){
	return RET_FAILURE;
    }
    //input
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    //update scene
    scene_next_action(_scene);
    
    //update player
    player_update(_scene->player, keyboardState, &_scene->projectiles_container);

    //update all enemies
    scene_update_enemies(&_scene->enemies_container, &_scene->projectiles_container);
  
    //update projectiles
    scene_update_projectiles(&_scene->projectiles_container);
    
    _scene->tick++;

    return RET_SUCCESS;
}

/**
 * Update all projectiles in a scene
 * @param _scene Scene containing a list of all projectiles
 * @return In case of succes returns 0, in case pointers parameters are invalid returns -1
 */
int scene_update_projectiles(game_container* projectiles_container){
    if(projectiles_container == NULL || projectiles_container->array == NULL)
	return RET_FAILURE;

    for(int i = 0; i < projectiles_container->count; i++){

	if( projectile_update(projectiles_container->array[i]) == RET_DEAD){
	    container_remove_destroy(projectiles_container, i);
	}
    
    }
  
    return RET_SUCCESS;
}


/**
 * Update all enemies on the current scene
 * @param enemies_container A pointer to the enemies_container structure
 where the enemies are located
 * @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
int scene_update_enemies(game_container* _enemies_container, game_container* projectiles_container){
    if(_enemies_container == NULL || projectiles_container == NULL){
	printf("NULL pointer at scene_update_enemes\n");
	return RET_FAILURE;
    }

    for(int i = 0; i < _enemies_container->count; i++){
	if(enemy_update(_enemies_container->array[i], projectiles_container) == RET_DEAD){
	    container_remove_destroy(_enemies_container, i);
	}
    }
  
    return 0;
}



/**
 * Draw all scene entities and background
 * @param _scene 
 * @param _renderer
 * @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
int scene_draw(scene_t* _scene, SDL_Renderer* _renderer){  
    if(_scene == NULL || _renderer == NULL)
	return RET_FAILURE;
  
    //background
    SDL_RenderCopy(_renderer, _scene->textures_vector[TXT_BG_DARKPURPLE], NULL, NULL);

    //draw spline grid
    if(false){
	int n = 25; //number of grids
	for(int i = 0; i < (WINDOW_WIDTH / n); i++){
	    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 150);
	    SDL_RenderDrawLine(_renderer,0, i * n, 600, i * n);

	    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 150);
	    SDL_RenderDrawLine(_renderer,i * n, 0, i * n, 600);
	}
    }

    //draw projectiles
    scene_draw_projectiles(&_scene->projectiles_container, _renderer);

    //draw enemies
    scene_draw_enemies(&_scene->enemies_container, _renderer);

    //draw the player
    player_draw(_scene->player, _renderer);
    
    //draw ui
    draw_ui(_scene->player, _scene->textures_vector,  _renderer);

    //draw paths
    for(int i = 0; i < _scene->spline_count; i++){
	spline_draw(_scene->splines[i], _renderer);
    }
    
    return RET_SUCCESS;
}



/**
 * Draw on screen all present projectiles on a scene
 * @param _scene Scene with a structure where all projectiles are stored
 * @param _renderer SDL2 renderer where everything will be drawn
 * @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
int scene_draw_projectiles(game_container* projectiles_container, SDL_Renderer* _renderer){
    if(_renderer == NULL)
	return RET_FAILURE;

    for(int i = 0; i < projectiles_container->count; i++){
	projectile_draw(projectiles_container->array[i], _renderer);
    }

    return RET_SUCCESS;
}


/**
 * Draws all enemies on current scene
 * @param _enemies_container A container structure where all enemies are located
 * @param _renderer A SDL2 Renderer where everything will be drawn
 * @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
int scene_draw_enemies(game_container* _enemies_container, SDL_Renderer* _renderer){
    if(_enemies_container == NULL || _renderer == NULL){
	return RET_FAILURE;
    }
  
    for(int i = 0; i < _enemies_container->count; i++){
	enemy_draw(_enemies_container->array[i], _renderer);
    }

    return RET_SUCCESS;
}


/**
 * Destroy scene and free it's space
 * @param _scene Scene to free
 * @return In case of succes returns 0, in case pointers parameters are invalid returns -1
 */
int scene_destroy(scene_t* _scene){
    if(_scene == NULL)
	return RET_FAILURE;

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
    container_clear(&_scene->enemies_container);
  
    free(_scene);
  
    return RET_SUCCESS;
}

void scene_next_action(scene_t* _scene){
    if(_scene == NULL) return;
    /* SDL_assert(_scene->current_keyframe > _scene->keyframe_count); */
    /* SDL_assert(_scene->keyframe_count < 0); */
    keyframe_t current_keyframe = _scene->keyframes[_scene->current_keyframe];
        
    if(_scene->tick == current_keyframe.tick){
	switch(current_keyframe.action){

	case KEYFRAME_ENEMY_ADD:{
	    //update max_enemy_id
	    _scene->max_enemy_id += 1;

	    //create enemy and assing max id to it
	    enemy_t * e = enemy_create(_scene->textures_vector, _scene->max_enemy_id);
	    container_add(&_scene->enemies_container, (void*)e);	    
	}break;

	case KEYFRAME_ENEMY_CHANGE_PATH:{
	    enemy_t** e = (enemy_t**)_scene->enemies_container.array;
	    int indices = _scene->enemies_container.count;

	    for(int i = 0; i < indices; i++){
		if(e[i]->id == current_keyframe.params.id && current_keyframe.params.pathid >= 0 && current_keyframe.params.pathid < _scene->spline_count){
		    enemy_change_path(e[i], &_scene->splines[current_keyframe.params.pathid]);
		}
	    }
	    
	    
	}break;
	    
	case KEYFRAME_ENEMY_DESTROY:{
	    enemy_t** e = (enemy_t**)_scene->enemies_container.array;
	    int indices = _scene->enemies_container.count;
	    
	    for(int i = 0; i < indices; i++){
		if(e[i]->id == current_keyframe.params.id){
		    container_remove_destroy(&_scene->enemies_container, i);
		    break;
		}
	    }
	}break;
	    
	default:{
	    printf("Invalid action on tick: %d\n", _scene->tick);
	};
	    
	}

	_scene->current_keyframe += 1;
    }
    
}

bool scene_load_level(const char* file_path, scene_t *_scene){
    //scene exists
    if(_scene == NULL){
	printf("Invalid pointer to scene\n");
	return false;
    }

    //initialize paths info
    memset(_scene->splines, 0, sizeof(_scene->splines));
    _scene->spline_count = 0;
    
    //open file
    FILE* level_file = fopen(file_path, "rb");
    if(level_file == NULL){
	printf("Failed opening level: %s", file_path);
	return false;
    }

    //check PATHS section
    char buff[32];
    fread(buff, sizeof(char), 6, level_file);

    if(strcmp(buff, "PATHS\n") != 0){
	printf("Path section not found\n");
	return false;
    }

    //check for a new path
    char c = getc(level_file);
    while(c == 'P'){
	printf("Found a new path\n");

	//structure to fill
	spline_t s = {0, {}, 0};

	//get total amount of points
	fread(&s.total_points, sizeof(size_t), 1, level_file);

	//get all the points in file
	fread(s.points, sizeof(SDL_FPoint), s.total_points, level_file);

	//get loop value
	fread(&s.loop, sizeof(bool), 1, level_file);

	//add path to array
	_scene->splines[_scene->spline_count] = s;
	_scene->spline_count += 1;
	
	c = getc(level_file);
    }
    
    //check for keyframe section
    
    fclose(level_file);

    return true;
    
    _scene->spline_count = 0;
    for(int i = 0; i < MAX_SPLINES; i++){
	_scene->splines[i].total_points = 0;
	memset(_scene->splines[i].points, 0, sizeof(SDL_FPoint) * MAX_POINTS);
	_scene->splines[i].loop         = 0;
    }
    SDL_FPoint p[] = { {0.0f, 0.5f}, {0.25f, 0.5f}, {0.50f, 0.50f}, {0.75f, 0.50f} };
    
    if(spline_add_points(&_scene->splines[0], p, 5)){
	_scene->spline_count += 1;
    }
    
    //load keyframes
    _scene->keyframe_count   = 0;
    _scene->current_keyframe = 0;

    scene_add_keyframe(_scene, keyframe_create_enemy(50, ENEMY_TYPE_NORMAL, -1));
    scene_add_keyframe(_scene, keyframe_create_enemy(75, ENEMY_TYPE_NORMAL, -1));
    scene_add_keyframe(_scene, keyframe_change_enemy_path(80, 1, 1));
    scene_add_keyframe(_scene, keyframe_destroy_enemy(200, 1));
    scene_add_keyframe(_scene, keyframe_destroy_enemy(300, 2));

    return 0;
}

void scene_add_keyframe(scene_t* _scene, keyframe_t keyframe){
    _scene->keyframes[_scene->keyframe_count] = keyframe;
    _scene->keyframe_count += 1;
}
