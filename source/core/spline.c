#include"spline.h"
#include <SDL2/SDL_render.h>
#include <string.h>

/**
 *Get's a position based on a spline and float in the range of 0.0 to 1.0.
 *@param _spline The spline structure where to get the points from.
 *@param _t The float which represent an percentage of the spline

 */
SDL_FPoint spline_get_point(spline_t _spline, float _t){
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

spline_t spline_create(bool _isLoop){
    spline_t nw_spline;

    nw_spline.total_points = 0;
    memset(nw_spline.points, 0, sizeof(nw_spline.points));
    nw_spline.loop = _isLoop;

    return nw_spline;
}

bool spline_add_point(spline_t *s, SDL_FPoint point){
    if(s->total_points < MAX_POINTS){
	s->points[s->total_points] = point;
	s->total_points += 1;
    }else{
	printf("Warning: spline max points reached!\n");
	return false;
    }

    return true;
}

bool spline_add_points(spline_t *s, SDL_FPoint *points, int size){
    //if spaces exist before and after adittions
    if(s->total_points < MAX_POINTS && s->total_points + size <= MAX_POINTS){
	for(int i = 0; i < size; i++){
	    s->points[s->total_points] = points[i];
	    s->total_points += 1;
	}	
    }else{
	printf("Spline_add_points failed: trying to add more points than allowed");
	return false;
    }

    return true;
}

SDL_FPoint to_worldCoords(SDL_FPoint point){
    SDL_FPoint p = {
	point.x * WINDOW_WIDTH,
	point.y * WINDOW_HEIGHT};
    return p;
}

SDL_FPoint to_fixedCoords(SDL_FPoint point){
    SDL_FPoint p = {
	point.x == 0 ? 0 : point.x / WINDOW_WIDTH,
	point.y == 0 ? 0 : point.y / WINDOW_HEIGHT
    };
    return p;
}

//basic draw function fallbacks to fixed draw
void spline_draw(spline_t _spline, SDL_Renderer* renderer){
    spline_draw_world(_spline, (SDL_Point){0, 0}, renderer);
}

// this functions expects splines points to be
// on (0, 1) range
void spline_draw_world(spline_t _spline, SDL_Point position, SDL_Renderer *renderer) {
    if(_spline.total_points < 4) return;
    
    float maxit = 0.0f;
  
    if(_spline.loop){
	maxit = _spline.total_points; 
    }else{
	maxit = _spline.total_points - 3.0f; 
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for(float i = 0.0f; i < maxit; i += +0.01f){
	SDL_FPoint p = to_worldCoords(spline_get_point(_spline, i));
	SDL_RenderDrawPointF(renderer, p.x, p.y);
    }
}

void spline_draw_camera(spline_t _spline, SDL_Point camera, SDL_Renderer* renderer){
    if(_spline.total_points < 4) return;
    
    float maxit = 0.0f;
  
    if(_spline.loop){
	maxit = _spline.total_points; 
    }else{
	maxit = _spline.total_points - 3.0f; 
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for(float j = 0.0f; j < maxit; j += +0.01f){
	//SDL_FPoint p = to_worldCoords(spline_get_point(_spline, i));
	SDL_FPoint p = spline_get_point(_spline, j);
	SDL_RenderDrawPointF(renderer,
			     p.x * WINDOW_WIDTH - camera.x,
			     p.y * WINDOW_HEIGHT - camera.y);
    }    
}

//this functions expects points to be
// on (0, WINDOW_WIDHT) range
void spline_draw_fixed(spline_t _spline, SDL_Point position, SDL_Renderer *renderer){
    if(_spline.total_points < 4) return;
    
    float maxit = 0.0f;
  
    if(_spline.loop){
	maxit = _spline.total_points; 
    }else{
	maxit = _spline.total_points - 3.0f; 
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for(float i = 0.0f; i < maxit; i += +0.01f){
	SDL_FPoint p = spline_get_point(_spline, i);
	SDL_RenderDrawPointF(renderer, p.x + position.x, p.y + position.y);
    }    
}

void spline_clean(spline_t *s){
    s->total_points = 0;
    s->loop = 0;

    for(int i = 0; i < MAX_POINTS; i++){
	s->points[i] = (SDL_FPoint){0.0, 0.0};
    }
}
