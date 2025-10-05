#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

//size 8 bytes
typedef struct vec2 {
	float x;
	float y;
} vec2;

#define create_vec2(x, y) \
	(vec2) { x, y }
#define summ_vec2(a, b) \
	(vec2) { a.x + b.x, a.y + b.y }
#define sub_vec2(a, b) \
	(vec2) { a.x - b.x, a.y - b.y }
#define vec2_sqr_len(a) (a.x * a.x + a.y * a.y)
#define vec2_len(a) (sqrtf(vec2_sqr_len(a)))
#define vec2_scalar(a, b) (a.x * b.x + a.y * b.y)

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

#define create_vec3(x, y, z) \
	(vec3) { x, y, z }
#define summ_vec3(a, b) \
	(vec3) { a.x + b.x, a.y + b.y, a.z + b.z }
#define sub_vec3(a, b) \
	(vec3) { a.x - b.x, a.y - b.y, a.z - b.z }
#define vec3_sqr_len(a) (a.x * a.x + a.y * a.y + a.z * a.z)
#define vec3_len(a) (sqrtf(vec3_sqr_len(a)))
#define vec3_scalar(a, b) (a.x * b.x + a.y * b.y + a.z * b.z)

void vec3_move_to(vec3 *this, vec3 a);
void vec3_move_toF(vec3 *this, float x, float y, float z);

void vec3_move_on(vec3 *this, vec3 a);
void vec3_move_onF(vec3 *this, float x, float y, float z);

#define vec2_to_vec3(vec) create_vec3(vec.x, vec.y, 0.0f)

//size 16 bytes
typedef struct rect {
	float x, y, w, h;
} rect;

#define create_rect(x, y, w, h) \
	(rect) { x, y, w, h }
#define create_rect_from_vec2(start, size) \
	(rect) { start.x, start.y, size.x, size.y }

inline bool check_collision(const rect pos1, const rect pos2) {
	return ((pos1.x + pos1.w) > pos2.x) && (pos1.x < (pos2.x + pos2.w)) && ((pos1.y + pos2.h) > pos2.y) && (pos1.y < (pos2.y + pos2.h));
}

#endif