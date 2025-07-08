#include "vector.h"

#include <math.h>

vec2 create_vec2(float x, float y) {
	return (vec2) {x, y};
}
vec2 summ_vec2(vec2 a, vec2 b) {
	return (vec2) {a.x + b.x, a.y + b.y};
}
vec2 sub_vec2(vec2 a, vec2 b) {
	return (vec2) {a.x - b.x, a.y - b.y};
}
float vec2_sqr_len(vec2 a) {
	return a.x * a.x + a.y * a.y;
}
float vec2_len(vec2 a) {
	return sqrtf(vec2_sqr_len(a));
}
float vec2_scalar(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}

void vec2_move_to(vec2 *this, vec2 a) {
	vec2_move_toF(this, a.x, a.y);
}
void vec2_move_toF(vec2 *this, float x, float y) {
	this->x = x;
	this->y = y;
}

void vec2_move_on(vec2 *this, vec2 a) {
	vec2_move_onF(this, a.x, a.y);
}
void vec2_move_onF(vec2 *this, float x, float y) {
	this->x += x;
	this->y += y;
}


vec3 create_vec3(float x, float y, float z) {
	return (vec3) {x, y, z};
}
vec3 summ_vec3(vec3 a, vec3 b) {
	return (vec3) {a.x + b.x, a.y + b.y, a.z + b.z};
}
vec3 sub_vec3(vec3 a, vec3 b) {
	return (vec3) {a.x - b.x, a.y - b.y, a.z - b.z};
}
float vec3_sqr_len(vec3 a) {
	return a.x * a.x + a.y * a.y + a.z * a.z;
}
float vec3_len(vec3 a) {
	return sqrtf(vec3_sqr_len(a));
}
float vec3_scalar(vec3 a, vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

void vec3_move_to(vec3 *this, vec3 a) {
	vec3_move_toF(this, a.x, a.y, a.z);
}
void vec3_move_toF(vec3 *this, float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void vec3_move_on(vec3 *this, vec3 a) {
	vec3_move_onF(this, a.x, a.y, a.z);
}
void vec3_move_onF(vec3 *this, float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;
}

vec3 vec2_to_vec3(vec2 vec) {
	return create_vec3(vec.x, vec.y, 0);
}

rect create_rect(float x, float y, float w, float h) {
	return (rect) {x, y, w, h};
}
rect create_rect_from_vec2(vec2 start, vec2 size) {
	return (rect) {start.x, start.y, size.x, size.y};
}


bool check_collision(const rect pos1, const rect pos2) {
	return ((pos1.x + pos1.w) > pos2.x) && (pos1.x < (pos2.x + pos2.w)) && ((pos1.y + pos2.h) > pos2.y) && (pos1.y < (pos2.y + pos2.h));
}