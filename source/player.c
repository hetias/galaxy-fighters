#include"player.h"


/**
 *Allocates memory for an player structure.
 *@params _texturesVector An vector of SDL_Textures pointers.
 *@return Pointer to an player structure.
 */
player_t* player_create(SDL_Texture**_textures_vector){

    player_t* tmp_player = (player_t*)malloc(sizeof(player_t));

    SDL_Point dims = {0, 0};
    SDL_QueryTexture(_textures_vector[TXT_PLAYER_BLUE],
		     NULL,
		     NULL,
		     &dims.x,
		     &dims.y);
  
    tmp_player->hp        = 5;
  
    tmp_player->position = (SDL_FPoint) {300.0f, 600.0f - dims.y};
    
    tmp_player->direction = (SDL_FPoint){0};

    tmp_player->hitbox    = (SDL_FRect){
	tmp_player->position.x,
	tmp_player->position.y,
	(float)dims.x,
	(float)dims.y};
    
    tmp_player->shootDelay = SHOOT_SLOW;
    tmp_player->currentShootDelay = tmp_player->shootDelay;
    tmp_player->speed     = 8.5f;

    tmp_player->sprite = _textures_vector[TXT_PLAYER_BLUE];
    tmp_player->projectile_sprite = _textures_vector[TXT_LASER_BLUE];

    return tmp_player;
}


/**
 *Updates player's internal state
 *@params _player Pointer to the player structure to update
 *@params _keyboardState Internal SDL2 array for keyboard input handling
 *@params _projectileList An list containing all current projectiles
 */

int  player_update(player_t* _player, const Uint8* _keyboardState, game_container* projectiles_container){

    if(_player == NULL || _keyboardState == NULL || projectiles_container == NULL){
	printf("Null pointer found at player_update\n");
	return RET_FAILURE;
    }
    
    if(_player->hp < 1){
	return RET_DEAD;
    }
    //get inputs

    //move vertically
    if(_keyboardState[SDL_SCANCODE_W]){
	if(_player->direction.y >= 0){
	    _player->direction.y -= 1;
	}
    }else if(_keyboardState[SDL_SCANCODE_S]){
	if(_player->direction.y <= 0){
	    _player->direction.y += 1;
	}
    }else{
	_player->direction.y = 0;
    }

    //move horizontally
    if(_keyboardState[SDL_SCANCODE_A]){
	if(_player->direction.x >= 0){
	    _player->direction.x -= 1;
	}
    }else if(_keyboardState[SDL_SCANCODE_D]){
	if(_player->direction.x <= 0){
	    _player->direction.x  += 1;
	}
    }else{
	_player->direction.x = 0;
    }

    //shoot projectile
    if(_keyboardState[SDL_SCANCODE_J] && _player->currentShootDelay < 0)
    {
	projectile_t* tmp_prj = projectile_create(_player->position, (SDL_FPoint){0.0f, -1.0f}, true, _player->projectile_sprite);

	container_add(projectiles_container, (void*)tmp_prj);
    
	_player->currentShootDelay = _player->shootDelay;
    }
    else
    {
	_player->currentShootDelay--;
    }

    //check collision with enemy projectiles
    if(!container_empty(*projectiles_container)){
	for(int i = 0; i < projectiles_container->count; i++){
	    projectile_t *prj = projectiles_container->array[i];

	    if(!prj->isFriendly){

		//for some reason i cant make a call for SDL_HasIntersectionF...so a little bit of magic here...
		//TODO::Make good use of the float values here, maybe just implementing an aabb collision check
		SDL_Rect a = {(int)prj->hitbox.x, (int)prj->hitbox.y, (int)prj->hitbox.w, (int)prj->hitbox.h};
		SDL_Rect b = {(int)_player->hitbox.x, (int)_player->hitbox.y, (int)_player->hitbox.w, (int)_player->hitbox.h};

		if(SDL_HasIntersection(&a, &b)){
		    _player->hp -= 1;
		    container_remove_destroy(projectiles_container, i);
		}
	    }
	}
    }

    //normalizing vector
    float magnitud = sqrt(_player->direction.x * _player->direction.x + _player->direction.y * _player->direction.y);
    _player->direction.x = (_player->direction.x != 0.0) ? _player->direction.x / magnitud : 0.0f;
    _player->direction.y = (_player->direction.y != 0.0) ? _player->direction.y / magnitud : 0.0f;
  
    //move
    _player->position.x += _player->direction.x * _player->speed;
    _player->position.y += _player->direction.y * _player->speed;

    _player->hitbox.x = _player->position.x - (_player->hitbox.w / 2);
    _player->hitbox.y = _player->position.y - (_player->hitbox.h / 2);

    return RET_ALIVE;
}



/**
 *Updates player's internal state
 *@params _player Pointer to the player structure to draw
 *@params _renderer Pointer to an SDL_Renderer where the player will be drawn
 */

void player_draw(player_t* _player, SDL_Renderer* _renderer){
    if(_player->sprite){
      
	/* SDL_SetRenderDrawColor(_renderer, 255, 0, 0 ,255); */
	/* SDL_RenderDrawRectF(_renderer, &_player->hitbox); */

	SDL_RenderCopyF(_renderer, _player->sprite, NULL, &_player->hitbox);
    }else{
	printf("Player has no texture assigned!");
    }
}

/**
 *Free's player resources 
 *@params _player Pointer to the player to be freed
 */

void player_destroy(player_t* _player)
{
    if(_player)
    {
	_player->sprite = NULL;
	_player->projectile_sprite = NULL;
    
	free(_player);
	_player = NULL;
    }
  
}
