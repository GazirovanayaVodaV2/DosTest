#include "vector.h"

#include <math.h>


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