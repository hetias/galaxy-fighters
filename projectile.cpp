#include"projectile.hpp"

projectile_t* projectile_create(SDL_FPoint _position, SDL_FPoint _direction)
{
  projectile_t* tmp = new projectile_t;

  tmp->position = _position;
  tmp->direction = _direction;
  tmp->hitbox = {13.0f, 13.0f};
  tmp->sprite = gTextures.at(TXT_LASER_BLUE);
  tmp->speed = 8.0f;
  tmp->isFriendly = false;

  return tmp;
}

void projectile_update(projectile_t* _prj)
{
  //prj->position.x += prj->direction.x * prj->speed;
  //prj->position.y += prj->direction.y * prj->speed;
}

void projectile_draw(projectile_t* _prj)
{
  SDL_FRect dst = {
    _prj->position.x,
    _prj->position.y,
    13.0f,
    54.0f};

  //drawing projectile
  SDL_RenderCopyF(gRenderer, _prj->sprite, NULL, &dst);

  //drawing dst
  SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
  SDL_RenderDrawRectF(gRenderer, &dst);

  //drawing hitbox
  SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
  SDL_RenderDrawRectF(gRenderer, &_prj->hitbox);
  
}

void projectiles_destroy(projectile_t* _prj)
{
  if(_prj)
  {
    delete _prj;
    _prj = NULL;
  }
}
