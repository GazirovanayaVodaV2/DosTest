#ifndef GAME_H
#define GAME_H

#include "../graphics/graph.h"
#include "../vector/vector.h"
#include <stddef.h>

typedef struct static_game_object {
	vec2 pos;
	size_t atlas_i;//index for global atlas
} static_game_object;

static_game_object *new_static_game_object(vec2 pos, size_t i);
void free_static_game_object(static_game_object *this);

void draw_static_game_object(static_game_object *this, double_buffer *db, atlas *atl);

typedef struct animated_game_object {
	vec2 pos;
	atlas *sprites;
	animation_player *anims;
	byte current_animation;
} animated_game_object;

animated_game_object *new_animated_game_object(vec2 pos);
void free_animated_game_object(animated_game_object *this);
void draw_animated_game_object(animated_game_object *this, double_buffer *db, float fps);

void push_sprite_to_animated_game_object(animated_game_object *this, image *sprite);
void push_animation_to_animated_game_object(animated_game_object *this, animation *anims);

typedef struct base_entity {
	animated_game_object *sprite;
	s16 hp;
} base_entity;

#endif