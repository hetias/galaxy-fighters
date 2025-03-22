#ifndef TIMING_H
#define TIMING_H

#include<SDL2/SDL_timer.h>
#include <stdint.h>

typedef struct time_info{
    uint64_t frame_start;
    uint64_t frame_end;
    Uint64 elasped;
}time_info;

void timeframe_start(time_info*);
void timeframe_end(time_info *);

Uint64 time_get_elapsed(time_info*);
double time_get_delta(time_info*);

#endif
