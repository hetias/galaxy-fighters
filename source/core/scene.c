#include "scene.h"

/**
 * Allocates memory for a new scene structure
 * @param _texture_paths
 * @return pointer to a new scene_t
 */
scene_t* scene_create(resources_t *_resources, const char* level_file){
    scene_t* new_scene = (scene_t*)malloc(sizeof(scene_t));

    //no resources loaded
    new_scene->resources = _resources;

    //create the player
    new_scene->player = player_create(new_scene->resources->textures);

    //create container for projectiles
    new_scene->projectiles_container = container_create(sizeof(projectile_t));

    //create enemy container
    new_scene->enemies_container = container_create(sizeof(enemy_t));
    new_scene->max_enemy_id = 0;

    //scene actions
    if(level_file != NULL){
	scene_load_level(level_file, new_scene);
    }else{
	new_scene->spline_count = 0;
	new_scene->keyframe_count = 0;
    }

    //start ticks
    new_scene->tick = 0;
    return new_scene;
}

/**
 * Update scenario and all it's current entities
 * @param _scene Scene to update
 * @return In case of succes returns 0, in case pointers parameters are invalid returns -1
 */
int scene_update(scene_t* _scene){
    if(_scene == NULL){
	return -1;
    }
    //input
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    //update scene
    scene_next_action(_scene);

    //update projectiles
    scene_update_projectiles(&_scene->projectiles_container,
			     &_scene->enemies_container,
			     _scene->player);
    
    //update player
    if(_scene->player->hp > 0){
	player_update(_scene->player,
		      keyboardState,
		      &_scene->projectiles_container);
    }

    //update all enemies
    scene_update_enemies(&_scene->enemies_container,
			 &_scene->projectiles_container);
    
    _scene->tick++;

    //print current tick as debug info
    char buff[32] = "";
    snprintf(buff, 32, "tick: %d", _scene->tick);

    debug_text(buff, 0, 0, (SDL_Color){0, 255, 0});

    return 0;
}

/**
 * Update all projectiles in a scene
 * @param _scene Scene containing a list of all projectiles
 * @return In case of succes returns 0, in case pointers parameters are invalid returns -1
 */
int scene_update_projectiles(array_list* _projectiles_container, array_list* _enemies_container, player_t* _player){
    for(int i = 0; i < _projectiles_container->len; i++){
	projectile_t* _prj = (projectile_t*)container_get(_projectiles_container, i);

	projectile_update(_prj, _enemies_container, _player);

	if(_prj->alive == false){
	    container_remove(_projectiles_container, i);
	    continue;
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
int scene_update_enemies(array_list* _enemies_container, array_list* projectiles_container){
    if(_enemies_container == NULL || projectiles_container == NULL){
	printf("NULL pointer at scene_update_enemes\n");
	return -1;
    }

    for(int i = 0; i < _enemies_container->len; i++){
	enemy_t* _enemy = (enemy_t*)container_get(_enemies_container, i);
	enemy_update(_enemy, projectiles_container);

	if(_enemy->hp < 1){
	    container_remove(_enemies_container, i);
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
	return -1;

    //background
    SDL_RenderCopy(_renderer, _scene->resources->textures[TXT_BG_DARKPURPLE], NULL, NULL);

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

    //draw enemies
    scene_draw_enemies(&_scene->enemies_container, _renderer);

    //draw projectiles
    scene_draw_projectiles(&_scene->projectiles_container, _renderer);

    //draw the player
    player_draw(_scene->player, _renderer);

    //draw ui
    //draw_ui(_scene->player, _scene->textures_vector,  _renderer);

    //draw paths
    for(int i = 0; i < _scene->spline_count; i++){
	spline_draw(_scene->splines[i], _renderer);
    }

    //text
    //SDL_Point p = {0.0, 0.0};
    //draw_text(_renderer, p, _scene->game_font, "this is a text");
    return 0;
}



/**
 * Draw on screen all present projectiles on a scene
 * @param _scene Scene with a structure where all projectiles are stored
 * @param _renderer SDL2 renderer where everything will be drawn
 * @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
int scene_draw_projectiles(array_list* projectiles_container,
                           SDL_Renderer* _renderer){
    if(_renderer == NULL)
	return -1;

    for(int i = 0; i < projectiles_container->len; i++){
	projectile_draw((projectile_t*)container_get(projectiles_container, i),
			_renderer);
    }

    return 0;
}


/**
 * Draws all enemies on current scene
 * @param _enemies_container A container structure where all enemies are located
 * @param _renderer A SDL2 Renderer where everything will be drawn
 * @return Returns 0 on succes, in case there are invalid pointers it returns -1
 */
int scene_draw_enemies(array_list* _enemies_container,
                       SDL_Renderer* _renderer){
    if(_enemies_container == NULL || _renderer == NULL){
	return -1;
    }

    for(int i = 0; i < _enemies_container->len; i++){
	enemy_draw((enemy_t*)container_get(_enemies_container, i),
		   _renderer);
    }

    return 0;
}


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

	if(_scene->resources->textures[i] != NULL){
	    SDL_DestroyTexture(_scene->resources->textures[i]);
	    _scene->resources->textures[i] = NULL;
	}

    }

    //destroy container
    container_clear(&_scene->projectiles_container);

    //destroy player
    player_destroy(_scene->player);

    //destroy enemies
    container_clear(&_scene->enemies_container);

    free(_scene);

    return 0;
}

void scene_next_action(scene_t* _scene){
    if(_scene == NULL) return;

    keyframe_t current_keyframe = _scene->keyframes[_scene->current_keyframe];

    if(_scene->tick == current_keyframe.tick){
#if DEBUG
	printf("hit keyframe tick: %d\n", _scene->tick);
#endif
	switch(current_keyframe.action){

	case KEYFRAME_ENEMY_ADD:{
	    //update max_enemy_id
	    _scene->max_enemy_id += 1;
	    
	    //create enemy and assing max id to it
	    enemy_t e = enemy_create(_scene->resources->textures, _scene->max_enemy_id);
	    enemy_change_path(&e, &_scene->splines[current_keyframe.params.pathid]);
	    container_add(&_scene->enemies_container, (void*)&e);
	}break;

	case KEYFRAME_ENEMY_CHANGE_PATH:{
	    for(int i = 0; i < _scene->enemies_container.len; i++){
		enemy_t *_enemy = (enemy_t*)container_get(&_scene->enemies_container, i);

		if(_enemy->id == current_keyframe.params.id &&
		   current_keyframe.params.pathid >= 0 &&
		   current_keyframe.params.pathid < _scene->spline_count){

		    enemy_change_path(_enemy,
				      &_scene->splines[current_keyframe.params.pathid]);
		    break;
		}

	    }
	}break;

	case KEYFRAME_ENEMY_DESTROY:{
	    for(int i = 0; i < _scene->enemies_container.len; i++){
		enemy_t *_enemy = (enemy_t*) container_get(&_scene->enemies_container, i);
		if(_enemy->id == current_keyframe.params.id){
		    container_remove(&_scene->enemies_container, i);
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

    //open file
    FILE* level_file = fopen(file_path, "rb");
    if(level_file == NULL){
	printf("Failed opening level: %s", file_path);
	return false;
    }

    //initialize paths info
    memset(_scene->splines, 0, sizeof(_scene->splines));
    _scene->spline_count = 0;

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

    //clean buffer
    memset(buff, 0, sizeof(buff));

    //initialize keyframe info
    _scene->current_keyframe = 0;
    _scene->keyframe_count = 0;
    memset(_scene->keyframes, 0, sizeof(_scene->keyframes));

    //check for keyframe section
    fread(buff, sizeof(char), 10, level_file);
    if(strcmp(buff, "KEYFRAMES\n") != 0){
	printf("Keyframe section not found\n");
	printf("c: %c\n", c);
	printf("buff: %s\n", buff);
    }

    //extract all keyframes
    c = getc(level_file);
    while(c == 'K'){
	keyframe_t k = {};
	fread(&k, sizeof(keyframe_t), 1, level_file);

	//TODO:::::::::::::::
	//we're multiplying the ticks manually by 60, so we move one tick
	//every 60 frames. This shouldn't be handled by the scene but
	//rather the level editor.

	//with vsync enabled this is not need...
	k.tick *= 1;

	_scene->keyframes[_scene->keyframe_count] = k;
	_scene->keyframe_count += 1;

	c = getc(level_file);
    }

    fclose(level_file);
    return true;
}

void scene_add_keyframe(scene_t* _scene, keyframe_t keyframe){
    _scene->keyframes[_scene->keyframe_count] = keyframe;
    _scene->keyframe_count += 1;
}
