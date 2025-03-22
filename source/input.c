#include "input.h"

void input_update(keyinfo_t* _keys){
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);
    int size = sizeof(_keys) / sizeof(keyinfo_t);

    for(int i = 0; i < size; i++){
	if(keyboard[_keys[i].key]){
	    if(_keys[i].state & INPUT_NONE){
		_keys[i].state |= INPUT_PRESSED;
	    }

	    if(_keys[i].state & INPUT_PRESSED){
		_keys[i].state |= INPUT_HELD;
	    }

	    if(_keys[i].state & INPUT_HELD){
		_keys[i].state |= INPUT_HELD;
	    }

	    if(_keys[i].state & INPUT_RELEASED){
		_keys[i].state |= INPUT_RELEASED;
	    }
	}else{
	    if(_keys[i].state & INPUT_NONE){

	    }

	    if(_keys[i].state & INPUT_PRESSED){
		_keys[i].state |= INPUT_RELEASED;
	    }

	    if(_keys[i].state & INPUT_HELD){
		_keys[i].state |= INPUT_RELEASED;
	    }

	    if(_keys[i].state & INPUT_RELEASED){
		_keys[i].state |= INPUT_NONE;
	    }
	}

	printf("%d: %b\n", i, _keys[i].state);
    }
}


void input_clear(keyinfo_t* _keys){
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);
    int size = sizeof(_keys) / sizeof(keyinfo_t);

    for(int i = 0; i < size; i++){
	if(_keys[i].state & INPUT_NONE){
	    _keys[i].state |= INPUT_NONE;
	    _keys[i].state &= ~INPUT_PRESSED;
	    _keys[i].state &= ~INPUT_RELEASED;
	    _keys[i].state &= ~INPUT_HELD;
	}

	if(_keys[i].state & INPUT_PRESSED){
	    _keys[i].state &= ~(INPUT_PRESSED);
	}

	if(_keys[i].state & INPUT_HELD){
	    _keys[i].state |= INPUT_HELD;
	}

	if(_keys[i].state & INPUT_RELEASED){
	    _keys[i].state &= ~(INPUT_RELEASED);
	}
    }
}


bool input_pressed(keyinfo_t* keys, SDL_Scancode key){
    int size = sizeof(keys) / sizeof(keyinfo_t);
    for(int i = 0; i < size; i++){
	if(key == keys->key){
	    return (keys->state & INPUT_PRESSED);
	}
    }
}

bool input_held(keyinfo_t* keys, SDL_Scancode key){
    int size = sizeof(keys) / sizeof(keyinfo_t);
    for(int i = 0; i < size; i++){
	if(key == keys->key){
	    return (keys->state & INPUT_HELD);
	}
    }
}

bool input_released(keyinfo_t* keys, SDL_Scancode key){
    int size = sizeof(keys) / sizeof(keyinfo_t);
    for(int i = 0; i < size; i++){
	if(key == keys->key){
	    return (keys->state & INPUT_RELEASED);
	}
    }
}

bool input_unpressed(keyinfo_t* keys, SDL_Scancode key){
    int size = sizeof(keys) / sizeof(keyinfo_t);
    for(int i = 0; i < size; i++){
	if(key == keys->key){
	    return (keys->state & INPUT_NONE);
	}
    }
}

