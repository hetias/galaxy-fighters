#include"../include/ui.h"

void draw_ui(player_t* _player, SDL_Texture **textures, SDL_Renderer* renderer){

    float time[5] = {1.0f};
    int x_separation = 33;
    int y_gap = 15;
    int x_gap = 5;
    SDL_Rect life_rect = {WINDOW_WIDTH - x_separation, y_gap, 33, 26};

    //draw heach hp icon
    for(int i = 0; i < _player->hp; i++){
	SDL_RenderCopy(renderer, textures[TXT_UI_LIFE], NULL, &life_rect);
	life_rect.x -= x_separation + x_gap;
    }
    
}

void draw_text(SDL_Renderer* renderer){
    
}

SDL_Point to_ui_coords(SDL_Point ui_coords){
    
}

SDL_Point to_screen_coords(SDL_Point screen_coords){
    
}

