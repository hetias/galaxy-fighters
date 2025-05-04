#include "input.h"

void input_clear(input_state *state){
    
}

void input_update(input_state *state){
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    
    if(keyboard_state[SDL_SCANCODE_W]){
	switch(state->move_up){
	case INPUT_UNPRESSED:
	    state->move_up = INPUT_PRESSED;
	    break;
	case INPUT_PRESSED:
	    state->move_up = INPUT_HELD;
	    break;
	case INPUT_HELD:
	    state->move_up = INPUT_HELD;
	    break;
	case INPUT_RELEASED:
	    state->move_up = INPUT_PRESSED;
	    break;
	}	
    }else{
	switch(state->move_up){
	case INPUT_UNPRESSED:
	    state->move_up = INPUT_UNPRESSED;
	    break;
	case INPUT_PRESSED:
	    state->move_up = INPUT_RELEASED;
	    break;
	case INPUT_HELD:
	    state->move_up = INPUT_RELEASED;
	    break;
	case INPUT_RELEASED:
	    state->move_up = INPUT_UNPRESSED;
	    break;
	}
    }
}
