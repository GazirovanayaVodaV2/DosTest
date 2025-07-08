#include "fps.h"

fps_counter create_fps_counter() {
	fps_counter fps;
	fps.frame_count = 0;
	fps.fps = 60;

	fps.start_time = clock();

	return fps;
}
float get_fps(fps_counter *this) {
	this->frame_count++;

	this->current_time = clock();

	if ((this->current_time - this->start_time) >= CLOCKS_PER_SEC) {
		this->fps = (float) this->frame_count / ((this->current_time - this->start_time) / (float) CLOCKS_PER_SEC);
		this->frame_count = 0;
		this->start_time = clock();
	}

	return this->fps;
}
