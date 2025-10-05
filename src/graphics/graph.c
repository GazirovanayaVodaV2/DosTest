#include "graph.h"
#include "sys/nearptr.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <dpmi.h>

static int clamp(int val, int min, int max) {
	return val < min ? min : (val > max ? max : val);
}

void set_video_mode(byte mode) {
	__dpmi_regs regs = {0};
	regs.x.ax = mode;
	__dpmi_int(0x10, &regs);
}

color new_color_vga(byte vga256) {
	color c;
	c.vga256 = vga256;
	return c;
}

color new_color(byte r, byte g, byte b) {
	color c;

	c.vga256 = rgb_to_vga256(r, g, b);

	return c;
}

color new_color_from_hex(uint32_t hex) {
	byte r = (hex >> 16) & 0xff;
	byte g = (hex >> 8) & 0xff;
	byte b = hex & 0xff;
	return new_color(r, g, b);
}

color new_color_from_hex_string(const char *hex_string) {
	return new_color_from_hex(get_hex_from_string(hex_string));
}

byte rgb_to_vga256(byte r, byte g, byte b) {
	/*
    https://gist.github.com/harieamjari/509f665081f52b3dbdfc892b39cc3eab
    */

	byte vga_palette[256][3] = {
			/* colors 0-15 */
			{0x00, 0x00, 0x00},

			{0x00, 0x00, 0xAA},
			{0x00, 0xAA, 0x00},
			{0x00, 0xAA, 0xAA},
			{0xAA, 0x00, 0x00},
			{0xAA, 0x00, 0xAA},

			{0xAA, 0x55, 0x00},
			{0xAA, 0xAA, 0xAA},
			{0x55, 0x55, 0x55},
			{0x55, 0x55, 0xFF},
			{0x55, 0xFF, 0x55},

			{0x55, 0xFF, 0xFF},
			{0xFF, 0x55, 0x55},
			{0xFF, 0x55, 0xFF},
			{0xFF, 0xFF, 0x55},
			{0xFF, 0xFF, 0xFF},

			/* grayscale 16-31 (non gamma corrected  */
			{0x00, 0x00, 0x00},
			{0x14, 0x14, 0x14},
			{0x20, 0x20, 0x20},
			{0x2C, 0x2C, 0x2C},
			{0x38, 0x38, 0x38},

			{0x45, 0x45, 0x45},
			{0x51, 0x51, 0x51},
			{0x61, 0x61, 0x61},
			{0x71, 0x71, 0x71},
			{0x82, 0x82, 0x82},

			{0x92, 0x92, 0x92},
			{0xA2, 0xA2, 0xA2},
			{0xB6, 0xB6, 0xB6},
			{0xCB, 0xCB, 0xCB},
			{0xE3, 0xE3, 0xE3},

			{0xFF, 0xFF, 0xFF},
			// HERE ------> 1
			/* hue mix 32-55 (1  */
			{0x00, 0x00, 0xFF},
			{0x41, 0x00, 0xFF},
			{0x7D, 0x00, 0xFF},
			{0xBE, 0x00, 0xFF},

			{0xFF, 0x00, 0xFF},
			{0xFF, 0x00, 0xBE},
			{0xFF, 0x00, 0x7D},
			{0xFF, 0x00, 0x41},
			{0xFF, 0x00, 0x00},

			{0xFF, 0x41, 0x00},
			{0xFF, 0x7D, 0x00},
			{0xFF, 0xBE, 0x00},
			{0xFF, 0xFF, 0x00},
			{0xBE, 0xFF, 0x00},

			{0x7D, 0xFF, 0x00},
			{0x41, 0xFF, 0x00},
			{0x00, 0xFF, 0x00},
			{0x00, 0xFF, 0x41},
			{0x00, 0xFF, 0x7D},

			{0x00, 0xFF, 0xBE},
			{0x00, 0xFF, 0xFF},
			{0x00, 0xBE, 0xFF},
			{0x00, 0x7D, 0xFF},
			{0x00, 0x41, 0xFF},

			/* hue mix 56-79 (2  */
			{0x7D, 0x7D, 0xFF},
			{0x9E, 0x7D, 0xFF},
			{0xBE, 0x7D, 0xFF},
			{0xDF, 0x7D, 0xFF},
			{0xFF, 0x7D, 0xFF},

			{0xFF, 0x7D, 0xDF},
			{0xFF, 0x7D, 0xBE},
			{0xFF, 0x7D, 0x9E},
			{0xFF, 0x7D, 0x7D},
			{0xFF, 0x9E, 0x7D},

			{0xFF, 0xBE, 0x7D},
			{0xFF, 0xDF, 0x7D},
			{0xFF, 0xFF, 0x7D},
			{0xDF, 0xFF, 0x7D},
			{0xBE, 0xFF, 0x7D},

			{0x9E, 0xFF, 0x7D},
			{0x7D, 0xFF, 0x7D},
			{0x7D, 0xFF, 0x9E},
			{0x7D, 0xFF, 0xBE},
			{0x7D, 0xFF, 0xDF},

			{0x7D, 0xFF, 0xFF},
			{0x7D, 0xDF, 0xFF},
			{0x7D, 0xBE, 0xFF},
			{0x7D, 0x9E, 0xFF},
			/* hue mix 80-103 (3  */
			{0xB6, 0xB6, 0xFF},

			{0xC7, 0xB6, 0xFF},
			{0xDB, 0xB6, 0xFF},
			{0xEB, 0xB6, 0xFF},
			{0xFF, 0xB6, 0xFF},
			{0xFF, 0xB6, 0xEB},

			{0xFF, 0xB6, 0xDB},
			{0xFF, 0xB6, 0xC7},
			{0xFF, 0xB6, 0xB6},
			{0xFF, 0xC7, 0xB6},
			{0xFF, 0xDB, 0xB6},

			{0xFF, 0xEB, 0xB6},
			{0xFF, 0xFF, 0xB6},
			{0xEB, 0xFF, 0xB6},
			{0xDB, 0xFF, 0xB6},
			{0xC7, 0xFF, 0xB6},

			{0xB6, 0xFF, 0xB6},
			{0xB6, 0xFF, 0xC7},
			{0xB6, 0xFF, 0xDB},
			{0xB6, 0xFF, 0xEB},
			{0xB6, 0xFF, 0xFF},

			{0xB6, 0xEB, 0xFF},
			{0xB6, 0xDB, 0xFF},
			{0xB6, 0xC7, 0xFF},
			// HERE ------> 2
			/* hue mix 104-127 (4  dark 1 */
			{0x00, 0x00, 0x71},
			{0x1C, 0x00, 0x71},
			{0x38, 0x00, 0x71},
			{0x55, 0x00, 0x71},

			{0x71, 0x00, 0x71},
			{0x71, 0x00, 0x55},
			{0x71, 0x00, 0x38},
			{0x71, 0x00, 0x1C},
			{0x71, 0x00, 0x00},

			{0x71, 0x1C, 0x00},
			{0x71, 0x38, 0x00},
			{0x71, 0x55, 0x00},
			{0x71, 0x71, 0x00},
			{0x55, 0x71, 0x00},

			{0x38, 0x71, 0x00},
			{0x1C, 0x71, 0x00},
			{0x00, 0x71, 0x00},
			{0x00, 0x71, 0x1C},
			{0x00, 0x71, 0x38},

			{0x00, 0x71, 0x55},
			{0x00, 0x71, 0x71},
			{0x00, 0x55, 0x71},
			{0x00, 0x38, 0x71},
			{0x00, 0x1C, 0x71},

			/* hue mix 56-79 (2  */
			{0x38, 0x38, 0x71},
			{0x45, 0x38, 0x71},
			{0x55, 0x38, 0x71},
			{0x61, 0x38, 0x71},
			{0x71, 0x38, 0x71},

			{0x71, 0x38, 0x61},
			{0x71, 0x38, 0x55},
			{0x71, 0x38, 0x45},
			{0x71, 0x38, 0x38},
			{0x71, 0x45, 0x38},

			{0x71, 0x55, 0x38},
			{0x71, 0x61, 0x38},
			{0x71, 0x71, 0x38},
			{0x61, 0x71, 0x38},
			{0x55, 0x71, 0x38},

			{0x45, 0x71, 0x38},
			{0x38, 0x71, 0x38},
			{0x38, 0x71, 0x45},
			{0x38, 0x71, 0x55},
			{0x38, 0x71, 0x61},

			{0x38, 0x71, 0x71},
			{0x38, 0x61, 0x71},
			{0x38, 0x55, 0x71},
			{0x38, 0x45, 0x71},
			/* hue mix 80-103 (3  */
			{0x51, 0x51, 0x71},

			{0x59, 0x51, 0x71},
			{0x61, 0x51, 0x71},
			{0x69, 0x51, 0x71},
			{0x71, 0x51, 0x71},
			{0x71, 0x51, 0x69},

			{0x71, 0x51, 0x61},
			{0x71, 0x51, 0x59},
			{0x71, 0x51, 0x51},
			{0x71, 0x59, 0x51},
			{0x71, 0x61, 0x51},

			{0x71, 0x69, 0x51},
			{0x71, 0x71, 0x51},
			{0x69, 0x71, 0x51},
			{0x61, 0x71, 0x51},
			{0x59, 0x71, 0x51},

			{0x51, 0x71, 0x51},
			{0x51, 0x71, 0x59},
			{0x51, 0x71, 0x61},
			{0x51, 0x71, 0x69},
			{0x51, 0x71, 0x71},

			{0x51, 0x69, 0x71},
			{0x51, 0x61, 0x71},
			{0x51, 0x59, 0x71},
			// HERE ------> 3
			/* hue mix 104-127 (4  dark 1 */
			{0x00, 0x00, 0x41},
			{0x10, 0x00, 0x41},
			{0x20, 0x00, 0x41},
			{0x30, 0x00, 0x41},

			{0x41, 0x00, 0x41},
			{0x41, 0x00, 0x30},
			{0x41, 0x00, 0x20},
			{0x41, 0x00, 0x10},
			{0x41, 0x00, 0x00},

			{0x41, 0x10, 0x00},
			{0x41, 0x20, 0x00},
			{0x41, 0x30, 0x00},
			{0x41, 0x41, 0x00},
			{0x30, 0x41, 0x00},

			{0x20, 0x41, 0x00},
			{0x10, 0x41, 0x00},
			{0x00, 0x41, 0x00},
			{0x00, 0x41, 0x10},
			{0x00, 0x41, 0x20},

			{0x00, 0x41, 0x30},
			{0x00, 0x41, 0x41},
			{0x00, 0x30, 0x41},
			{0x00, 0x20, 0x41},
			{0x00, 0x10, 0x41},

			/* hue mix 56-79 (2  */
			{0x20, 0x20, 0x41},
			{0x28, 0x20, 0x41},
			{0x30, 0x20, 0x41},
			{0x3C, 0x20, 0x41},
			{0x41, 0x20, 0x41},

			{0x41, 0x20, 0x3C},
			{0x41, 0x20, 0x30},
			{0x41, 0x20, 0x28},
			{0x41, 0x20, 0x20},
			{0x41, 0x28, 0x20},

			{0x41, 0x30, 0x20},
			{0x41, 0x3C, 0x20},
			{0x41, 0x41, 0x20},
			{0x3C, 0x41, 0x20},
			{0x30, 0x41, 0x20},

			{0x28, 0x41, 0x20},
			{0x20, 0x41, 0x20},
			{0x20, 0x41, 0x28},
			{0x20, 0x41, 0x30},
			{0x20, 0x41, 0x3C},

			{0x20, 0x41, 0x41},
			{0x20, 0x3C, 0x41},
			{0x20, 0x30, 0x41},
			{0x20, 0x28, 0x41},
			/* hue mix 80-103 (3  */
			{0x2C, 0x2C, 0x41},

			{0x30, 0x2C, 0x41},
			{0x34, 0x2C, 0x41},
			{0x3C, 0x2C, 0x41},
			{0x41, 0x2C, 0x41},
			{0x41, 0x2C, 0x3C},

			{0x41, 0x2C, 0x34},
			{0x41, 0x2C, 0x30},
			{0x41, 0x2C, 0x2C},
			{0x41, 0x30, 0x2C},
			{0x41, 0x34, 0x2C},

			{0x41, 0x3C, 0x2C},
			{0x41, 0x41, 0x2C},
			{0x3C, 0x41, 0x2C},
			{0x34, 0x41, 0x2C},
			{0x30, 0x41, 0x2C},

			{0x2C, 0x41, 0x2C},
			{0x2C, 0x41, 0x30},
			{0x2C, 0x41, 0x34},
			{0x2C, 0x41, 0x3C},
			{0x2C, 0x41, 0x41},

			{0x2C, 0x3C, 0x41},
			{0x2C, 0x34, 0x41},
			{0x2C, 0x30, 0x41},

			/* all black */
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0},

			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0},

	};

	s32 sqr_closest = INT32_MAX;
	byte ndx = 0;

	for (byte i = 0; i < 248; i++) {
		byte *sample = vga_palette[i];
		byte rs = sample[0];
		byte gs = sample[1];
		byte bs = sample[2];
		s32 sqr_dst =
				((rs - r) * (rs - r)) + ((gs - g) * (gs - g)) + ((bs - b) * (bs - b));

		if (sqr_closest > sqr_dst) {
			sqr_closest = sqr_dst;
			ndx = i;
		}
	}
	return (byte) ndx;
}

u32 get_hex_from_string(const char *hex_string) {
	if (hex_string[0] != '#' || hex_string == NULL) {
		return 0;
	}

	if (strlen(hex_string) != 7) {
		return 0;
	}

	u32 res = (u32) strtoul(hex_string + 1, NULL, 16);
	if (res == ULONG_MAX) {
		return 0xff000000;
	} else if (res == 0) {
		return 0x00000000;
	}

	return res;
}

vram create_vram() {
	vram v;
	v.vram = (byte *) (__djgpp_conventional_base + 0xa0000);
	return v;
}

double_buffer *new_double_buffer(byte video_mode) {
	double_buffer *db = new(double_buffer);

	if (db == NULL) {
		print_error(("Double buffer allocation error!"), MEMORY_ALLOCATION_ERROR);
	}

	db->front = create_vram();
	db->back = (byte *) malloc(sizeof(byte) * VRAM_SIZE);
	if (db->back == NULL) {
		print_error(("Back buffer allocation error!"), MEMORY_ALLOCATION_ERROR);
	}
	set_video_mode(video_mode);

	return db;
}

void free_double_buffer(double_buffer *db) {
#ifdef DEBUG
	printf("Double buffer deleting\n");
#endif

	if (db->back != NULL) {
		free(db->back);
	}

	if (db != NULL) {
		free(db);
	}

	set_video_mode(TEXT_MODE);
}

static void wait_for_vsync() {
	__dpmi_regs regs;


	//this should work mb
	regs.h.ah = 0x86;
	regs.x.cx = 0x00;
	regs.x.dx = 0x4268;

	__dpmi_int(0x15, &regs);
}

void clear_double_buffer(double_buffer *db) {
	memset(db->back, 0, VRAM_SIZE);
}

void present_double_buffer(double_buffer *db, bool vsync) {

	if (vsync) {
		wait_for_vsync();
	}
	memcpy(db->front.vram, db->back, VRAM_SIZE);
}


void set_pixel(double_buffer *db, s16 x, s16 y, color clr) {
	if ((x >= 0 && x < VRAM_W) && (y > 0 && y < VRAM_H)) {
		s32 index = x + y * (VRAM_W);
		if (db->back[index] != clr.vga256) {
			db->back[index] = clr.vga256;
		}
	}
}

void draw_rect(double_buffer *db, s16 x, s16 y, s16 w, s16 h, color clr) {
	w = clamp(w, 0, VRAM_W - x);
	h = clamp(h, 0, VRAM_H - y);
	for (s16 i = 0; i < w; i++) {
		for (s16 u = 0; u < h; u++) {
			set_pixel(db, x + i, y + u, clr);
		}
	}
}


image *new_image(const char *path) {
	printf("Loading img, path: %s\n", path);
	FILE *img_file = fopen(path, "rb");

	if (img_file != NULL) {

		image *img = new(image);

		if (img == NULL) {
			fclose(img_file);

			print_error(("Image allocation error!"), MEMORY_ALLOCATION_ERROR);
			return NULL;
		}

		fseek(img_file, 0, SEEK_END);
		long int num_bytes = ftell(img_file);
		fseek(img_file, 0, SEEK_SET);
		char *image_data = (char *) malloc(num_bytes + 1);

		if (image_data == NULL) {
			free(img);
			fclose(img_file);
			print_error(("Image data allocation error!"), MEMORY_ALLOCATION_ERROR);
		}

		fread(image_data, 1, num_bytes, img_file);
		image_data[num_bytes] = '\0';

		s32 count_of_pixels = num_bytes - 2;

		img->w = image_data[0];
		img->h = image_data[1];

		img->pixels = (color *) malloc(sizeof(color) * count_of_pixels);

		if (img->pixels == NULL) {
			free(img);
			free(image_data);
			fclose(img_file);

			print_error(("Image pixels allocation error!"), MEMORY_ALLOCATION_ERROR);
		}

		for (u32 i = 0; i < count_of_pixels; i++) {
			img->pixels[i] = new_color_vga((byte) image_data[i + 2]);
		}

		free(image_data);
		fclose(img_file);

		printf("Loaded\n");

		return img;
	}

	print_error(("Image file openning error!"), FILE_OPEN_ERROR);
	return NULL;
}

void free_image(image *img) {
	if (img != NULL) {
#ifdef DEBUG
		printf("Image deleting\n");
#endif
		free(img->pixels);
		free(img);
	}
}

void draw_image(double_buffer *db, image *img, s16 x, s16 y) {
	for (u32 i = 0; i < img->w * img->h; i++) {
		s16 _x = i % img->w;
		s16 _y = i / img->w;
		s16 pos_x = x + _x;
		s16 pos_y = y + _y;

		if (pos_y > 250) return;
		set_pixel(db, pos_x, pos_y, img->pixels[i]);
	}
}


atlas *new_atlas(byte size) {
	atlas *this = new(atlas);

	this->size = size;

	this->sprites = new_array();

	return this;
}

void free_atlas(atlas *this) {
#ifdef DEBUG
	printf("Atlas deleting\n");
#endif
	free_array(this->sprites, free_image);
	free(this);
	this = NULL;
}

void atlas_push(atlas *this, image *sprite) {
	array_push(this->sprites, sprite);
}
image *atlas_get(atlas *this, size_t i) {
	return (image *) array_get(this->sprites, i);
}

animation_key *new_animation_key(size_t i, float time) {
	animation_key *this = new(animation_key);
	if (this == NULL) {
		print_error(("Animation key allocation error!"), MEMORY_ALLOCATION_ERROR);
		return this;
	}

	this->atlas_i = i;
	this->time = time;

	return this;
}

void free_animation_key(animation_key *this) {
#ifdef DEBUG
	printf("Animation key deleting\n");
#endif
	free(this);
	this = NULL;
}

animation *new_animation() {
	animation *this = new(animation);
	if (this == NULL) {
		print_error(("Animation allocation error!"), MEMORY_ALLOCATION_ERROR);
		return this;
	}
	this->current_frame = 0;
	this->frame_counter = 0;
	this->keys = new_array();
	return this;
}

void free_animation(animation *this) {
#ifdef DEBUG
	printf("Animation deleting\n");
#endif
	free_array(this->keys, free_animation_key);
	free(this);
	this = NULL;
}

void push_animation_key(animation *this, animation_key *key) {
	array_push(this->keys, key);
}

animation_player *new_animation_player() {
	animation_player *this = new(animation_player);
	if (this == NULL) {
		print_error(("Animation player allocation error!"), MEMORY_ALLOCATION_ERROR);
		return this;
	}
	this->animations = new_array();
	return this;
}
void free_animation_player(animation_player *this) {
#ifdef DEBUG
	printf("Animation player deleting\n");
#endif
	free_array(this->animations, free_animation);
	free(this);
	this = NULL;
}
void push_animation(animation_player *this, animation *an) {
	array_push(this->animations, an);
}
size_t play_animation(animation_player *this, size_t an_index, float fps) {
	animation *current_animation = array_get(this->animations, an_index);

	animation_key *current_key = array_get(current_animation->keys, current_animation->current_frame);
	s32 need_frames = roundf(current_key->time * fps);

	++current_animation->frame_counter;
	if (current_animation->frame_counter > need_frames) {
		current_animation->frame_counter = 0;
		++current_animation->current_frame;

		if (current_animation->current_frame > (current_animation->keys->size - 1)) {
			current_animation->current_frame = 0;
		}
	}
	return current_animation->current_frame;
}

font *new_font(const char *path) {
	printf("Loading font: %s\n", path);

	FILE *font_file = fopen(path, "rb");
	if (font_file != NULL) {

		font *this = new(font);
		if (this == NULL) {
			fclose(font_file);
			print_error(("Font allocation error"), MEMORY_ALLOCATION_ERROR);
		}

		fseek(font_file, 0, SEEK_END);
		long int num_bytes = ftell(font_file);
		fseek(font_file, 0, SEEK_SET);
		byte *bytes = (byte *) malloc(num_bytes + 1);
		memset(bytes, 0, num_bytes + 1);
		if (bytes == NULL) {
			fclose(font_file);
			print_error(("Font data allocation error!"), MEMORY_ALLOCATION_ERROR);
		}

		fread(bytes, 1, num_bytes, font_file);
		bytes[num_bytes] = '\0';


		for (byte i = 0; i < 36; i++) {
			for (byte u = 0; u < 16; u++) {
				this->bytes[i][u] = 0;
				this->bytes[i][u] = bytes[i - (i > 9 ? 9 : 0) + (u * 9) + ((i / 9) * 144)];
			}
		}

		free(bytes);

		fclose(font_file);

		printf("Loaded\n");

		return this;
	}

	print_error(("Font file openning error!"), FILE_OPEN_ERROR);
	return NULL;
}
void free_font(font *this) {
	if (this != NULL) {
#ifdef DEBUG
		printf("Font deleting\n");
#endif
		free(this);
		return;
	}
	print_error(("Memory deallocation error!"), MEMORY_DEALLOCATION_ERROR);
}

void draw_letter(double_buffer *db, font *this, const char letter, s16 x, s16 y) {
	s16 index;

	if (letter >= 97) {
		index = letter - 32 - 65;
	} else if (letter >= 65) {
		index = letter - 65;
	} else {
		index = letter - 23;
	}

	byte *bytes = this->bytes[index];

	for (byte i = 0; i < 8; i++) {
		for (byte u = 0; u < 16; u++) {
			if ((bytes[u] >> (7 - i)) & 1) {
				set_pixel(db, x + i, y + u, new_color_vga(15));
			}
		}
	}
}

void draw_string(double_buffer *db, font *this, const char *str, s16 x, s16 y) {
	for (size_t i = 0; i < strlen(str); i++) {
		if (str[i] != ' ') {
			draw_rect(db, x + 16 * i - 2, y - 2, 13, 20, new_color_vga(0));
			if (str[i] == '0') {
				draw_rect(db, x + 16 * i + 1, y, 8, 16, new_color_vga(15));// bug with rendering 0
			} else {
				draw_letter(db, this, str[i], x + 16 * i + 1, y);
			}

			//draw_letter(db, this, str[i], x + 16 * i + 1, y);
		}
	}
}