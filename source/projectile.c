#include"projectile.h"

/**
 *Allocates memory for a new projectile structure
 *@params _position Initial enemy position on creation
 *@params _direction Initial enemy direction
 *@params _isFriendly Describes if the projectile should damage the player
 *@params _sprite Pointer to an SDL_Texture used for drawing the projectile
 *@return Pointer to an player structure.
 */

projectile_t* projectile_create(SDL_FPoint _position, SDL_FPoint _direction, bool _isFriendly, SDL_Texture* _sprite)
{
  projectile_t* tmp = (projectile_t*)malloc(sizeof(projectile_t));

  tmp->position = _position;
  tmp->direction = _direction;
  tmp->hitbox = (SDL_FRect){tmp->position.x, tmp->position.y,
                 13.0f,           54.0f};
  tmp->sprite = _sprite;
  tmp->speed = NORMAL_PRJ_SPEED;
  tmp->isFriendly = _isFriendly;

  return tmp;
}

/**
 *Updates the projectile position and state.
 *@params _prj The projectile to update.
 *@return In case of deletion return 1.
 */

int projectile_update(projectile_t* _prj){
  if(_prj == NULL)
    return RET_FAILURE;
  
  //on bounds check
  if(_prj->position.x > 600 || _prj->position.x < 0 ||
     _prj->position.y > 600 || _prj->position.y < 0){
    
    return RET_DEAD;
  }else{
    
    _prj->position.x += _prj->direction.x * _prj->speed;
    _prj->position.y += _prj->direction.y * _prj->speed;

    _prj->hitbox.x = _prj->position.x - (_prj->hitbox.w / 2);
    _prj->hitbox.y = _prj->position.y - (_prj->hitbox.h / 2);

    return RET_ALIVE;
  }
}

/**
 *Draws the projectile on current renderer.
 *@params _prj The projectile to draw.
 *@params _renderer Pointer to SDL_Renderer where the projectile will be drawn.
 */

void projectile_draw(projectile_t* _prj, SDL_Renderer* _renderer)
{
  //drawing projectile
  if(_prj->isFriendly){
    SDL_RenderCopyExF(_renderer,
                      _prj->sprite,
                      NULL,
                      &_prj->hitbox,
                      0.0,
                      NULL,
                      SDL_FLIP_NONE);
  }else{
    SDL_RenderCopyExF(_renderer,
                      _prj->sprite,
                      NULL,
                      &_prj->hitbox,
                      180.0,
                      NULL,
                      SDL_FLIP_NONE);
  }
  //drawing hitbox
  //FIX::The hitbox is correctly drawn for player projs but
  //not enemy projs
  /* SDL_FRect smallerHitbox = _prj->hitbox; */
  /* smallerHitbox.h = _prj->hitbox.h / 3.0f; */
  /* SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255); */
  /* SDL_RenderDrawRectF(_renderer, &smallerHitbox); */
  
}

/**
 *Frees the projectile from memory.
 *@params _prj The projectile to free
 */
void projectile_destroy(projectile_t** projectile){
  if(projectile == NULL && *projectile == NULL){
    printf("Failed to destroy projectiles\nFound void pointers on 'projectile_destroy'\n");    
  }else{
    free(*projectile);
    *projectile = NULL;
  }
}


