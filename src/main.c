#include "Array/array.h"
#include "pc.h"
#include <bios.h>
#include <dpmi.h>
#include <stddef.h>
#include <sys/nearptr.h>
#include <stdio.h>
#include <math.h>

#include "proto.h"

#include "graphics/graph.h"
#include "fps_timer/fps.h"
#include "stdlib.h"
#include "vector/vector.h"

#include "stdbool.h"
#include "vector/vector.h"

#include "game_logic/game.h"
#include "game_logic/levels.h"
#include "game_logic/assets.h"

int main(void) {

	atlas *global_atlas = load_assets(0x55);


	const float player_speed = 2.5f;
	vec2 spawn_player_pos = {0};


	level *test_lvs = load_level(0);

	spawn_player_pos.x = test_lvs->player_spawn_point.x * 32;
	spawn_player_pos.y = test_lvs->player_spawn_point.y * 32;

	base_entity player;
	player.hp = 100;
	player.sprite = new_animated_game_object(spawn_player_pos);

	vec2 *player_pos = &player.sprite->pos;

	push_sprite_to_animated_game_object(player.sprite, new_image("assets/plr0.vga"));
	push_sprite_to_animated_game_object(player.sprite, new_image("assets/plr1.vga"));
	push_sprite_to_animated_game_object(player.sprite, new_image("assets/plr2.vga"));
	push_sprite_to_animated_game_object(player.sprite, new_image("assets/plr3.vga"));
	animation *player_idle = new_animation();
	push_animation_key(player_idle, new_animation_key(0, 0.1f));

	animation *player_move = new_animation();
	push_animation_key(player_move, new_animation_key(1, 0.5f));
	push_animation_key(player_move, new_animation_key(2, 0.5f));
	push_animation_key(player_move, new_animation_key(3, 0.5f));

	push_animation_to_animated_game_object(player.sprite, player_idle);
	push_animation_to_animated_game_object(player.sprite, player_move);

	font *fnt = new_font("assets/font.bit");

	double_buffer *db = new_double_buffer(VGA_256);

	fps_counter fps = create_fps_counter();

	char key = 0;

	while (key != 27) {


		if (kbhit()) {
			key = getkey();
		} else {
			key = 0;

			player.sprite->current_animation = 0;
		}


		switch (key) {
			case ('w'): {
				//player.speed = create_vec2(0, -player_speed);
				player.sprite->current_animation = 1;
			} break;
			case ('s'): {
				//player.speed = create_vec2(0, player_speed);
				player.sprite->current_animation = 1;
			} break;
			case ('a'): {
				player.speed.x = -player_speed;
				player.sprite->current_animation = 1;
			} break;
			case ('d'): {
				player.speed.x = player_speed;
				player.sprite->current_animation = 1;
			} break;
			default: {

			} break;
		}

		clear_double_buffer(db);

		update_entity(&player, test_lvs);

		update_and_draw_level(db, test_lvs, global_atlas);
		draw_animated_game_object(player.sprite, db, fps.fps);


		char fps_str[4];
		itoa((int) roundf(fps.fps > 999 ? 999 : fps.fps),
			 fps_str, 10);
		draw_string(db, fnt, fps_str, 0, 10);

		present_double_buffer(db, false);

		get_fps(&fps);
	}
	free_double_buffer(db);

	unload_level(test_lvs);

	free_animated_game_object(player.sprite);

	free_font(fnt);
	free_atlas(global_atlas);

	//free_atlas(sprites);

#ifdef DEBUG
	key = 0;
	while (key != 27) {
		if (kbhit()) {
			key = getkey();
		} else {
			key = 0;
		}
	}
#endif

	return 0;
}