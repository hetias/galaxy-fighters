#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>

enum input_state{
    INPUT_UNPRESSED,
    INPUT_PRESSED,
    INPUT_HELD,
    INPUT_RELEASED,
};

typedef struct keyinfo_s{
    SDL_Scancode key;
    int state;
}keyinfo_t;

typedef struct input_state_s{
    int move_up;
    int move_down;
    int move_left;
    int move_right;
    int shoot;
    int pause;
    int escape;
}input_state;

void input_clear(input_state*);
void input_update(input_state*);

#endif
