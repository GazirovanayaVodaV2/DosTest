#ifndef ARRAY_H
#define ARRAY_H


#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../proto.h"

typedef struct array {
	void **data;
	size_t size;
} array;

array *new_array();

void free_array(array *this, destructor destr);

void array_push(array *this, void *data);
void *array_get(array *this, size_t index);

#define foreach(ITEM, ARRAY)                                               \
	for (size_t i = 0, _keep_ = true; i < ARRAY->size; i++, _keep_ = true) \
		for (ITEM = array_get(ARRAY, i); _keep_; _keep_ = false)

#endif
