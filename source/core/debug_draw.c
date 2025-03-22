#include"debug_draw.h"

void debug_start(){
  g_debug_info.top = 0;
  g_debug_info.text_surface = NULL;
  g_debug_info.text_texture = NULL;
}

void debug_clean(){
  g_debug_info.top = -1;

  if(g_debug_info.text_surface != NULL)
    SDL_FreeSurface(g_debug_info.text_surface);

  if(g_debug_info.text_texture != NULL)
    SDL_DestroyTexture(g_debug_info.text_texture);
}

void debug_rect(int _x, int _y, int _w, int _h, SDL_Color _color){
  debug_data_t d = {
    .type = RECT,
    .x = _x,
    .y = _y,
    .x2 = _w,
    .y2 = _h,
    .color = _color
  };

  g_debug_info.data[g_debug_info.top] = d;
  g_debug_info.top += 1;
}

void debug_line(int _x, int _y, int _x2, int _y2, SDL_Color _color){

  debug_data_t d = {
    .type = LINE,
    .x = _x,
    .y = _y,
    .x2 = _x2,
    .y2 = _y2,
    .color = _color
  };

  g_debug_info.data[g_debug_info.top] = d;
  g_debug_info.top += 1;
}

void debug_text(const char* _text, int _x, int _y, SDL_Color _color){

  debug_data_t d = {
    .type = TEXT,
    //.text = _text, not really
    .x = _x,
    .y = _y,
    .color = _color
  };

  int len = strlen(_text);
  for(int i = 0; i < len; i++){
    d.text[i] = _text[i];
  }
  d.text[len+1] = '\n';

  g_debug_info.data[g_debug_info.top] = d;
  g_debug_info.top += 1;
}

void debug_draw_present(){

  for(int i = 0; i < g_debug_info.top; i++){

    switch(g_debug_info.data[i].type){

    case RECT:
      debug_draw_rect(g_debug_info.data[i]);
      break;
    case LINE:
      debug_draw_line(g_debug_info.data[i]);
      break;
    case TEXT:
      debug_draw_text(g_debug_info.data[i]);
      break;
    }
  }

  g_debug_info.top = 0;
}


void debug_draw_rect(debug_data_t _data){
  SDL_Rect r = {_data.x, _data.y,
                _data.x2, _data.y2};

  SDL_SetRenderDrawColor(renderer, _data.color.r,
                         _data.color.g, _data.color.b,
                         0);
  SDL_RenderDrawRect(renderer, &r);
}

void debug_draw_line(debug_data_t _data){
  SDL_SetRenderDrawColor(renderer, _data.color.r,
                         _data.color.g, _data.color.b,
                         0);
  SDL_RenderDrawLine(renderer, _data.x, _data.y,
                     _data.x2, _data.y2);
}

void debug_draw_text(debug_data_t _data){
  if(_data.text == NULL)
    return;

  g_debug_info.text_surface = TTF_RenderText_Blended(game_resources.font, _data.text, _data.color);

  if(g_debug_info.text_surface != NULL)
    g_debug_info.text_texture = SDL_CreateTextureFromSurface(renderer, g_debug_info.text_surface);
  else
    return;

  if(g_debug_info.text_texture != NULL){
    SDL_Rect r = {_data.x, _data.y,
                  g_debug_info.text_surface->w, g_debug_info.text_surface->h};
    SDL_RenderCopy(renderer, g_debug_info.text_texture,
                   NULL, &r);

  }else{
    return;
  }
}
