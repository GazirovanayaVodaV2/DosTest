#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

//size 8 bytes
typedef struct vec2 {
	float x;
	float y;
} vec2;

vec2 create_vec2(float x, float y);
vec2 summ_vec2(vec2 a, vec2 b);
vec2 sub_vec2(vec2 a, vec2 b);
float vec2_sqr_len(vec2 a);
float vec2_len(vec2 a);
float vec2_scalar(vec2 a, vec2 b);

void vec2_move_to(vec2 *this, vec2 a);
void vec2_move_toF(vec2 *this, float x, float y);

void vec2_move_on(vec2 *this, vec2 a);
void vec2_move_onF(vec2 *this, float x, float y);

//size 12 bytes
typedef struct vec3 {
	float x;
	float y;
	float z;
} vec3;

vec3 create_vec3(float x, float y, float z);
vec3 summ_vec3(vec3 a, vec3 b);
vec3 sub_vec3(vec3 a, vec3 b);
float vec3_sqr_len(vec3 a);
float vec3_len(vec3 a);
float vec3_scalar(vec3 a, vec3 b);

void vec3_move_to(vec3 *this, vec3 a);
void vec3_move_toF(vec3 *this, float x, float y, float z);

void vec3_move_on(vec3 *this, vec3 a);
void vec3_move_onF(vec3 *this, float x, float y, float z);

vec3 vec2_to_vec3(vec2 vec);

//size 16 bytes
typedef struct rect {
	float x, y, w, h;
} rect;

rect create_rect(float x, float y, float w, float h);
rect create_rect_from_vec2(vec2 start, vec2 size);

bool check_collision(const rect pos1, const rect pos2);

#endif