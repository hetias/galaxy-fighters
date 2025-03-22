#include"timing.h"

void timeframe_start(time_info* timing){
    timing->frame_start = SDL_GetTicks64();
}
void timeframe_end(time_info* timing){
    timing->frame_end = SDL_GetTicks64();
}

Uint64 time_get_elapsed(time_info *timing){
  return (timing->frame_start - timing->frame_end) + 1;
}

double time_get_delta(time_info *timing){
  return time_get_elapsed(timing) / 1000.0f;
}
