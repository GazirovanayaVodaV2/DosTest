#ifndef LEVELS_H
#define LEVELS_H

#include <stdlib.h>
#include "../proto.h"
#include "../array/array.h"
#include "../vector/vector.h"
#include "../graphics/graph.h"

#define LEVEL_MAX_HEIGTH 32

enum OBJECT_TYPES {
	BRICK_WALL = 'W',
	WATER = 'w',
	EXIT = 'E',
	KEY = 'k',
	PLAYER_SPAWN = 'S',
	AIR = ' '
};


static const char *levels[][LEVEL_MAX_HEIGTH] = {
		{"WWWWWWWWWW",
		 "WS       W",
		 "W        W",
		 "W        W",
		 "W        W",
		 "W   k   EW",
		 "WWWWWWWWWW\n"}};

typedef struct level {
	size_t id;
	byte goal;
	array *static_objects;
	array *animated_objects;
	array *keys;
	array *enemies;
	vec2 player_spawn_point;
	u16 w, h;
	byte sprite_size;
} level;

level *load_level(size_t _i);
void unload_level(level *this);
void update_and_draw_level(double_buffer *db, level *this, atlas *global_atlas);

char get_level_objectC(vec2 pos, level *this);

//shitty code, but its C :(
u16 handle_object_collision(rect obj, level *this);


#endif