#include"timing.h"
#include <SDL2/SDL_timer.h>

void timeframe_start(time_info* timing){
    timing->frame_start = SDL_GetPerformanceCounter();
}
void timeframe_end(time_info* timing){
    timing->frame_end = SDL_GetPerformanceCounter();
}
uint64_t timeframe_cycles(time_info *timing) {
    return timing->frame_cycles = (timing->frame_end -
				   timing->frame_start);
}

float timeframe_ms(time_info *timing){
    return timing->frame_ms = (((float)timing->frame_end - (float)timing->frame_start) / (float)SDL_GetPerformanceFrequency()) * 1000.0f;
}

float timeframe_sc(time_info *timing){
    return timing->frame_ms = (((float)timing->frame_end - (float)timing->frame_start) / (float)SDL_GetPerformanceFrequency());
}
