#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>

void inputs_get();
void inputs_clean();

bool input_key_pressed(SDL_Scancode);
bool input_key_released(SDL_Scancode);
bool input_key_hold(SDL_Scancode);

bool input_mouse_pressed(int);
bool input_mouse_released(int);
bool input_mouse_hold(int);

SDL_Point input_mouse_position();
