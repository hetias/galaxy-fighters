#ifndef TIMING_H
#define TIMING_H

#include<SDL2/SDL_timer.h>
#include <stdint.h>

typedef struct time_info{
    uint64_t frame_start;
    uint64_t frame_end;
    uint64_t frame_cycles;
    float frame_ms;
    float frame_sc;
}time_info;

void timeframe_start(time_info*);
void timeframe_end(time_info *);

uint64_t timeframe_cycles(time_info*);
float timeframe_ms(time_info *);
float timeframe_sc(time_info *);

#endif
