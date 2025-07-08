#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include <stdbool.h>

#include "../proto.h"
#include "../array/array.h"
#include "../fps_timer/fps.h"

#define VRAM_SIZE 64000
#define VRAM_W 320
#define VRAM_H 200

#define VGA_256 0x13
#define TEXT_MODE 0x03

/*
TODO:
	1) Add support for multiple video modes

 Mode Resolution Colors T/G  CharBlock   AlphaRes
    ------------------------------------------------
    0,1  360x400    16     T    9x16        40x25
    2,3  720x400    16     T    9x16        80x25 - DONE
    4,5  320x200    4      G    8x8         40x25
    6    640x200    2      G    8x8         80x25
    7    720x400    mono   T    9x16        80x25
    D    320x200    16     G    8x8         40x25
    E    640x200    16     G    8x8         80x25
    F    640x350    mono   G    8x14        80x25
    10   640x350    16     G    8x14        80x25
    11   640x480    2      G    8x16        80x30
    12   640x480    16     G    8x16        80x30
    13   320x200    256    G    8x8         40x25 - DONE
*/

void set_video_mode(byte mode);

typedef struct color {
	byte vga256;
} color;

color new_color_vga(byte vga256);
color new_color(byte r, byte g, byte b);
color new_color_from_hex(uint32_t hex);
color new_color_from_hex_string(const char *hex_string);

byte rgb_to_vga256(byte r, byte g, byte b);
uint32_t get_hex_from_string(const char *hex_string);

typedef struct vram {
	byte *vram;
} vram;

vram create_vram();

typedef struct double_buffer {
	vram front;
	byte *back;
} double_buffer;

double_buffer *new_double_buffer(byte video_mode);
void free_double_buffer(double_buffer *db);

void present_static_buffer(double_buffer *db);
void present_double_buffer(double_buffer *db, bool vsync);

void set_pixel(double_buffer *db, s16 x, s16 y, color clr);
void draw_rect(double_buffer *db, s16 x, s16 y, s16 w, s16 h, color clr);

typedef struct image {
	color *pixels;
	byte w, h;
} image;

image *new_image(const char *path);
void free_image(image *img);

void draw_image(double_buffer *db, image *img, s16 x, s16 y);

typedef struct atlas {
	array *sprites;
	byte size;//0xab -w 2^a -h 2^b
} atlas;

atlas *new_atlas(byte size);
void free_atlas(atlas *this);

void atlas_push(atlas *this, image *sprite);
image *atlas_get(atlas *this, size_t i);

typedef struct animation_key {
	size_t atlas_i;
	float time;//time in seconds
} animation_key;

animation_key *new_animation_key(size_t i, float time);
void free_animation_key(animation_key *this);

typedef struct animation {
	size_t current_frame, frame_counter;
	array *keys;
} animation;

animation *new_animation();
void free_animation(animation *this);
void push_animation_key(animation *this, animation_key *key);

typedef struct animation_player {
	array *animations;
} animation_player;

animation_player *new_animation_player();
void free_animation_player(animation_player *this);
void push_animation(animation_player *this, animation *an);
size_t play_animation(animation_player *this, size_t an_index, float fps);//return atlas i

typedef struct font {
	/*
        36 symbols, 16 bytes per symbol
    */
	byte bytes[36][16];
} font;

font *new_font(const char *path);
void free_font(font *this);

void draw_letter(double_buffer *db, font *this, const char letter, s16 x, s16 y);
void draw_string(double_buffer *db, font *this, const char *str, s16 x, s16 y);

#endif