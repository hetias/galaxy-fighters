#include"projectile.hpp"

projectile_t* projectile_create(SDL_FPoint _position, SDL_FPoint _direction, bool _isFriendly, SDL_Texture* _sprite)
{
  projectile_t* tmp = new projectile_t;

  tmp->position = _position;
  tmp->direction = _direction;
  tmp->hitbox = {tmp->position.x, tmp->position.y,
                 13.0f,           54.0f};
  tmp->sprite = _sprite;
  tmp->speed = SLOW_PRJ_SPEED;
  tmp->isFriendly = _isFriendly;

  return tmp;
}

int projectile_update(projectile_t* _prj)
{

  //on bounds check
  if(_prj->position.x > 600 || _prj->position.x < 0 ||
     _prj->position.y > 600 || _prj->position.y < 0)
  {
    projectile_destroy(_prj);
    return 1;
  }else
  {
    _prj->position.x += _prj->direction.x * _prj->speed;
    _prj->position.y += _prj->direction.y * _prj->speed;

    _prj->hitbox.x = _prj->position.x - (_prj->hitbox.w / 2);
    _prj->hitbox.y = _prj->position.y - (_prj->hitbox.h / 2);

    return 0;
  }
}

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
  //SDL_FRect smallerHitbox = _prj->hitbox;
  //smallerHitbox.h = _prj->hitbox.h / 3.0f;
  //SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
  //SDL_RenderDrawRectF(gRenderer, &smallerHitbox);
  
}

void projectile_destroy(projectile_t* _prj)
{
  if(_prj)
  {
    delete _prj;
    _prj = NULL;
  }
}
