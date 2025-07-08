#ifndef FPS_H
#define FPS_H

#include <time.h>
#include "../proto.h"

typedef struct fps_counter {
	clock_t start_time, current_time;
	s16 frame_count;
	float fps;
} fps_counter;

fps_counter create_fps_counter();
float get_fps(fps_counter *fps);

#endif