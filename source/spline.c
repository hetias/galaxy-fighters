#include"spline.hpp"

/**
 *Get's a position based on a spline and float in the range of 0.0 to 1.0.
 *@param _spline The spline structure where to get the points from.
 *@param _t The float which represent an percentage of the spline

 */
SDL_FPoint spline_get_point(spline _spline, float _t){
  int p0, p1, p2, p3;

  if(_spline.loop){
    p1 = (int)_t;
    p2 = (p1 + 1) % _spline.total_points;
    p3 = (p2 + 1) % _spline.total_points;
    p0 = p1 >= 1 ? p1 - 1 : _spline.total_points - 1;
  }else{
    p1 = (int)_t + 1;
    p2 = p1 + 1;
    p3 = p2 + 1;
    p0 = p1 - 1;
  }

  _t = _t - (int)_t;

  float tt = _t * _t;
  float ttt = _t * _t * _t;

  float q1 = -ttt + 2.0f*tt - _t;
  float q2 = 3.0f*ttt - 5.0f*tt + 2.0f;
  float q3 = -3.0f*ttt + 4.0f*tt + _t;
  float q4 = ttt - tt;

  SDL_FPoint point;

  point.x = 0.5f * (_spline.points[p0].x * q1 + _spline.points[p1].x * q2 + _spline.points[p2].x * q3 + _spline.points[p3].x * q4);
  point.y = 0.5f * (_spline.points[p0].y * q1 + _spline.points[p1].y * q2 + _spline.points[p2].y * q3 + _spline.points[p3].y * q4);

  return point;
}

spline spline_create(bool _isLoop){
  spline nw_spline;

  nw_spline.total_points = 0;

  nw_spline.points;
  for(int i = 0; i < nw_spline.points; i++){
    nw_spline.points[i] = {0.0, 0.0};
  }

  nw_loop = _isLoop;

  return nw_spline;
}

void spline_add_point(spline *s, SDL_FPoint point){
  if(s->total_points < MAX_POINTS){
    s->points[total_points] = point;
    s->total_points += 1;
    printf("Warning: spline max points reached!\n");
  }
}

void spline_draw(spline _spline, SDL_Renderer* renderer){
  float maxit = 0.0f;
  
  if(_spline.loop){
    maxit = _spline.points.size(); 
  }else{
    maxit = _spline.points.size() - 3.0f; 
  }

  SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
  for(float i = 0.0f; i < maxit; i += +0.01f){
    SDL_FPoint p = spline_get_point(_spline, i);

    SDL_RenderDrawPointF(renderer, p.x, p.y);
  }
  
}

void spline_clean(spline* s){
  s->total_points = 0;
  s->loop = 0;

  for(int i = 0; i < MAX_POINTS; i++){
    s->points[i] = {0.0, 0.0};
  }
}
