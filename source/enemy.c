#include"enemy.h"
#include "player.h"
#include <SDL2/SDL_rect.h>

/**
 *Allocates memory for an enemy structure.
 *@params _texturesVector An vector of SDL_Textures pointers.
 *@return Pointer to an enemy structure.
 */

enemy_t* enemy_create(SDL_Texture** _texturesVector, int _id){
    enemy_t* enemy = (enemy_t*)malloc(sizeof(enemy_t));
    
    enemy->id = _id;
    enemy->position = (SDL_FPoint){0.0f, 0.0f};

    //get texture dimensions
    SDL_Point dimensions = (SDL_Point){0};
    SDL_QueryTexture(_texturesVector[TXT_ENEMY1_BLACK],
		     NULL,
		     NULL,
		     &dimensions.x,
		     &dimensions.y);

    enemy->hitbox   = (SDL_FRect){enemy->position.x,
	enemy->position.y,
	(float)dimensions.x,
	(float)dimensions.y};
    enemy->sprite   = _texturesVector[TXT_ENEMY1_BLACK];
    enemy->projectile_texture   = _texturesVector[TXT_LASER_RED];
    enemy->hp       = 4;
    enemy->shootDelay = SHOOT_SLOW;
    enemy->speed = 1.0f;
    enemy->currentDelay = enemy->shootDelay;
    enemy->path_time = 0.0f;
    enemy->path = NULL;
    enemy->path_state = PATH_STATE_FORWARD;
    enemy->path_type = PATH_REPEAT;
    return enemy;  
}

/**
 *Updates internal enemy values.
 *@params _projectileList A pointer to list containing all current game projectiles.
 */
int enemy_update(enemy_t* _enemy, game_container* projectiles_container){
    if(_enemy == NULL || projectiles_container == NULL){
	printf("Found Null pointer on enemy_update\n");
	return -1;
    }

    //shootin'
    if(_enemy->currentDelay <= 0 && _enemy->shootDelay > 0){
	projectile_t* tmp_prj = projectile_create(_enemy->position, (SDL_FPoint){0.0, 1.0},false,_enemy->projectile_texture);
	container_add(projectiles_container, tmp_prj);

	_enemy->currentDelay = _enemy->shootDelay;
    }else{
	_enemy->currentDelay -= 1;
    }
  
    //movin'
    //TODO:: Enemy doesn't have speed perse
    //becase of this there is no way to update
    //it's position  with delta time on a straight
    //forward way. How "fast" it moves it's determined
    //by spline_get_point and path_time.
    int up = enemy_update_path(_enemy);
    
    //check collisions with projectiles
    if(!container_empty(*projectiles_container)){
	projectile_t *prj = NULL;

	for(int i = 0; i < projectiles_container->count; i++){

	    prj = projectiles_container->array[i];
	    if(prj->isFriendly){

		//for some reason i cant make a call for SDL_HasIntersectionF...so a little bit of magic here...
		//TODO::Make good use of the float values here, maybe just implementing an aabb collision check
		SDL_Rect a = {(int)prj->hitbox.x, (int)prj->hitbox.y, (int)prj->hitbox.w, (int)prj->hitbox.h};
		SDL_Rect b = {(int)_enemy->hitbox.x, (int)_enemy->hitbox.y, (int)_enemy->hitbox.w, (int)_enemy->hitbox.h};
		if(SDL_HasIntersection(&a, &b)){
		    _enemy->hp -= 1;
		    //container_remove_destroy(projectiles_container, i); come one now
		    if(_enemy->hp < 1)			
			return -2;
		}
	    }
	}
    }

    //hitbox repositioning
    _enemy->hitbox.x = _enemy->position.x - _enemy->hitbox.w / 2;
    _enemy->hitbox.y = _enemy->position.y - _enemy->hitbox.h / 2;

    return 1;
}

/**
 *Updates enemy position based on it's current path and timing.
 *@params _enemy Pointer to the desired enemy to modify.
 *@params _spline Pointer to the new spline to assign to enemy.
 */

/* void enemy_change_path(enemy_t* _enemy, spline* _spline){ */
/*   _enemy->path = _spline; */
/* } */

/**
 *Draws the enemy texture to the game's renderer
 *@params _enemy Pointer to enemy to draw
 *@params _renderer Pointer to an SDL_Renderer where the player is drawn
 */

void enemy_draw(enemy_t* _enemy, SDL_Renderer* _renderer){
    if(_enemy){

	SDL_RenderCopyF(_renderer,
			_enemy->sprite,
			NULL,
			&_enemy->hitbox
	    );
    }

    #if DEBUG
   
    SDL_FRect _positionSquare = {_enemy->position.x - 5.0f,
	_enemy->position.y - 5.0f,
	10.0f,
	10.0f,
    };
  
    //draw centered cube
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
    SDL_RenderDrawRectF(gRenderer, &_positionSquare);

    //draw hitbox
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderDrawRectF(gRenderer, &_enemy->hitbox);

    //draw position point
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
    SDL_RenderDrawPointF(gRenderer, _enemy->position.x, _enemy->position.y);
    #endif
  
}

/**
 * Free's enemy resources from memory.
 * @params _enemy Pointer to enemy to be freed.
 */

void enemy_destroy(enemy_t** enemy){
    if(enemy == NULL && *enemy == NULL)  {
	printf("Failede on 'enemy_destroy', void pointer passed\n");
    }else{
	free(*enemy);
	*enemy = NULL;
    }
}

/**
 * Updates enemy position based on it's current path and timing.
 * @params _enemy The desired enemy to update.
 */

PATH_STATE enemy_update_path(enemy_t* _enemy){
    if(_enemy->path == NULL) return PATH_STATE_START;
    
    switch(_enemy->path_type){
    case PATH_START_END:
	if(_enemy->path_time >= 0.0 && _enemy->path_time <= 1.0f){
            _enemy->path_time += _enemy->speed * gametime.frame_ms;
	    _enemy->position = to_worldCoords(spline_get_point(*_enemy->path, _enemy->path_time));

	    if(_enemy->path_time >= 1.0f){
		return PATH_STATE_END;
	    }else if(_enemy->path_time <= 0.0f){
		return PATH_STATE_START;
	    }else{
		return PATH_STATE_FORWARD;
	    }
	}		
	break;
    case PATH_REPEAT:
	if(_enemy->path_time >= 0.0 && _enemy->path_time <= 1.0f){
	    if(_enemy->path_state == PATH_STATE_FORWARD){
		_enemy->path_time += _enemy->speed * gametime.frame_ms;
		_enemy->position = to_worldCoords(spline_get_point(*_enemy->path, _enemy->path_time));
		
		if(_enemy->path_time >= 1.0f){
		    _enemy->path_state = PATH_STATE_BACKWARD;
		    _enemy->path_time = 1.0f;
		    return PATH_STATE_END;
		}
	    }else if(_enemy->path_state == PATH_STATE_BACKWARD){
		_enemy->path_time -= _enemy->speed * gametime.frame_ms;
		_enemy->position = to_worldCoords(spline_get_point(*_enemy->path, _enemy->path_time));		
		
		if(_enemy->path_time <= 0.0f){
		    _enemy->path_state = PATH_STATE_FORWARD;
		    _enemy->path_time = 0.0f;
		    return PATH_STATE_START;
		}
	    }break;
	case PATH_LOOP:
	    if(_enemy->path->loop && _enemy->path_type == PATH_LOOP){
		if(_enemy->path_time >= 0.0 && _enemy->path_time <= 1.0f){
		    _enemy->path_time += _enemy->speed * gametime.frame_ms;
		    _enemy->position = to_worldCoords(spline_get_point(*_enemy->path, _enemy->path_time));
				
		    if(_enemy->path_time >= 1.0f){
			return PATH_STATE_END;
		    }else if(_enemy->path_time <= 0.0f){
			return PATH_STATE_START;
		    }else{
			return PATH_STATE_FORWARD;
		    }
		}		
	    }break;
	}
    }

    return PATH_STATE_FORWARD;
}

void enemy_change_path(enemy_t* _enemy, spline_t* _spline){
    _enemy->path = _spline;
}

void enemy_set_can_shoot(enemy_t* enemy, bool can_shoot, int speed){
    if(can_shoot){
	enemy->shootDelay = speed;
    }else{
	enemy->shootDelay = 0;
    }
}
