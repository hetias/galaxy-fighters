#include"spline.hpp"

SDL_FPoint spline_get_point(spline pSpline, float t){
  int p0, p1, p2, p3;

  if(pSpline.loop){
    p1 = (int)t;
    p2 = (p1 + 1) % pSpline.points.size();
    p3 = (p2 + 1) % pSpline.points.size();
    p0 = p1 >= 1 ? p1 - 1 : pSpline.points.size() -1;
  }else{
    p1 = (int)t + 1;
    p2 = p1 + 1;
    p3 = p2 + 1;
    p0 = p1 - 1;
  }

  t = t - (int)t;

  float tt = t * t;
  float ttt = t * t * t;

  float q1 = -ttt + 2.0f*tt - t;
  float q2 = 3.0f*ttt - 5.0f*tt + 2.0f;
  float q3 = -3.0f*ttt + 4.0f*tt + t;
  float q4 = ttt - tt;

  SDL_FPoint point;

  point.x = 0.5f * (pSpline.points[p0].x * q1 + pSpline.points[p1].x * q2 + pSpline.points[p2].x * q3 + pSpline.points[p3].x * q4);
  point.y = 0.5f * (pSpline.points[p0].y * q1 + pSpline.points[p1].y * q2 + pSpline.points[p2].y * q3 + pSpline.points[p3].y * q4);

  return point;
}

void spline_draw(spline pSpline){

  float maxit = 0.0f;
  
  if(pSpline.loop){
    maxit = pSpline.points.size(); 
  }else{
    maxit = pSpline.points.size() - 3.0f; 
  }

  SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
  for(float i = 0.0f; i < maxit; i += +0.01f){
    SDL_FPoint p = spline_get_point(pSpline, i);

    SDL_RenderDrawPointF(gRenderer, p.x, p.y);
  }
  
}

