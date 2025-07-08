#include "game.h"
#include "../proto.h"
#include <math.h>

static_game_object *new_static_game_object(vec2 pos, size_t i) {
	static_game_object *this = new (static_game_object);
	if (this == NULL) {
		print_error(("Static game object allocation error!"), MEMORY_ALLOCATION_ERROR);
		return this;
	}

	this->pos = pos;
	this->atlas_i = i;

	return this;
}
void free_static_game_object(static_game_object *this) {
#ifdef DEBUG
	printf("Static game object deleting\n");
#endif
	free(this);
	this = NULL;
}

void draw_static_game_object(static_game_object *this, double_buffer *db, atlas *atl) {
	draw_image(db, array_get(atl->sprites, this->atlas_i),
			   (s16) roundf(this->pos.x * (1 << (atl->size >> 4))), (s16) roundf(this->pos.y * (1 << (atl->size & 0x0f))));
}


animated_game_object *new_animated_game_object(vec2 pos) {
	animated_game_object *this = new (animated_game_object);
	if (this == NULL) {
		print_error(("Animated game object allocation error"), MEMORY_ALLOCATION_ERROR);
		return this;
	}

	this->anims = new_animation_player();
	this->sprites = new_atlas(0);
	this->current_animation = 0;
	this->pos = pos;

	return this;
}
void free_animated_game_object(animated_game_object *this) {
#ifdef DEBUG
	printf("Animated game object deleting\n");
#endif
	free_animation_player(this->anims);
	free_atlas(this->sprites);
	free(this);
	this = NULL;
}
void draw_animated_game_object(animated_game_object *this, double_buffer *db, float fps) {

	size_t i = play_animation(this->anims, this->current_animation, fps);
	draw_image(db, atlas_get(this->sprites, i),
			   (s16) roundf(this->pos.x), (s16) roundf(this->pos.y));
}

void push_sprite_to_animated_game_object(animated_game_object *this, image *sprite) {
	atlas_push(this->sprites, sprite);
}
void push_animation_to_animated_game_object(animated_game_object *this, animation *anims) {
	push_animation(this->anims, anims);
}
