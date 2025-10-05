#include "game.h"
#include "levels.h"
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "assets.h"


level *load_level(size_t _i) {
	level *this = new(level);

	if (this == NULL) {
		print_error(("Level allocation error!"), MEMORY_ALLOCATION_ERROR);
	}

	this->static_objects = new_array();
	this->animated_objects = new_array();

	this->sprite_size = 0xff;

	this->id = _i;
	//this->keys = new_array();
	//this->enemies = new_array();

	const char **level = levels[_i];

	for (size_t i = 0; i < LEVEL_MAX_HEIGTH; i++) {
		const char *layer = level[i];
		const size_t len = strlen(layer);
		this->w = len > this->w ? len : this->w;
		for (size_t u = 0; u < len; u++) {
			const char c = layer[u];
			if (c == '\n') {
				this->h = ++i;
				goto after_getting_height;
			}
		}
	}
after_getting_height:

	bool has_spawn = false;

	for (size_t i = 0; i < this->h; i++) {
		const char *layer = level[i];
		const size_t len = strlen(layer);
		for (size_t u = 0; u < len; u++) {
			const char c = layer[u];

			vec2 pos = create_vec2(u, i);
			switch (c) {
				case BRICK_WALL: {
					array_push(this->static_objects, new_static_game_object(pos, 0));
				} break;
				case EXIT: {
					array_push(this->static_objects, new_static_game_object(pos, 1));
				} break;
				case PLAYER_SPAWN: {
					if (has_spawn) {
						print_error(("Level already has spawn!"), OTHER_ERROR);
					}
					has_spawn = true;
					this->player_spawn_point = pos;
				} break;
				case KEY: {
					this->goal++;
				} break;
				case WATER: {

				} break;
				case AIR: {

				} break;
			}
		}
	}

	if (!has_spawn) {
		print_error(("No spawn point!"), OTHER_ERROR);
	}

	return this;
}

void unload_level(level *this) {
	free_array(this->static_objects, free_static_game_object);
	free_array(this->animated_objects, free_animated_game_object);
	//free_array(this->keys, );
	//free_array(this->enemies, );

	free(this);
	this = NULL;
}

void update_and_draw_level(double_buffer *db, level *this, atlas *global_atlas) {
	this->sprite_size = global_atlas->size;
	foreach (static_game_object *static_object, this->static_objects) {
		draw_static_game_object(static_object, db, global_atlas);
	}
}

char get_level_objectC(vec2 pos, level *this) {
	int x = roundf(pos.x) / (1 << ((this->sprite_size & 0xf0) >> 4));
	int y = roundf(pos.y) / (1 << (this->sprite_size & 0x0f));

	if ((x >= this->w) || (y >= this->h)) {
		return AIR;
	}

	if ((x < 0) || (y < 0) || (this->id < 0)) {
		return AIR;
	}

	return levels[this->id][y][x];
}

u16 handle_object_collision(rect obj, level *this) {
	vec2 obj_pos = create_vec2(obj.x, obj.y);
	const char left_obj = get_level_objectC(sub_vec2(obj_pos, create_vec2(obj.w, 0)), this);
	const char right_obj = get_level_objectC(summ_vec2(obj_pos, create_vec2(obj.w, 0)), this);
	const char top_obj = get_level_objectC(sub_vec2(obj_pos, create_vec2(0, obj.h)), this);
	const char bottom_obj = get_level_objectC(summ_vec2(obj_pos, create_vec2(0, obj.h)), this);


	const char objects[4] = {
			left_obj,
			right_obj,
			top_obj,
			bottom_obj};

	const enum SIDE sides[4] = {
			LEFT,
			RIGHT,
			TOP,
			BOTTOM};

	u16 result = 0x0000;

	for (size_t i = 0; i < 4; ++i) {
		const char object = objects[i];

		switch (object) {
			case BRICK_WALL: {
				result |= sides[i];
			} break;

			case AIR: {

			} break;
		}
	}

	return result;
}