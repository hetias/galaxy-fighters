#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>

#define INPUT_NONE     0b0001
#define INPUT_PRESSED  0x0002
#define INPUT_HELD     0x0003
#define INPUT_RELEASED 0x0004

typedef struct keyinfo_s{
	SDL_Scancode key;
	int state;
}keyinfo_t;

static keyinfo_t game_keys[] = {{SDL_SCANCODE_W, INPUT_NONE},
						 {SDL_SCANCODE_S, INPUT_NONE},
						 {SDL_SCANCODE_D, INPUT_NONE},
						 {SDL_SCANCODE_A, INPUT_NONE},
						 {SDL_SCANCODE_J, INPUT_NONE}};

void input_clear(keyinfo_t*);
void input_update(keyinfo_t*);

bool input_pressed(keyinfo_t*, SDL_Scancode);
bool input_held(keyinfo_t*, SDL_Scancode);
bool input_released(keyinfo_t*, SDL_Scancode);
bool input_unpressed(keyinfo_t*, SDL_Scancode);
#endif
