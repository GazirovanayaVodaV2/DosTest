#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../proto.h"

array *new_array() {
	array *this = new (array);

	this->data = new (void *);

	if (this->data == NULL) {
		print_error(("Array memory allocation error"), MEMORY_ALLOCATION_ERROR);
		return this;
	}

	this->size = 0;

	return this;
}

void free_array(array *this, destructor destr) {
	// Check if the array or destructor is NULL
	if (this == NULL) {
		print_error(("Null Array Error!"), NULL_ARRAY_ERROR);
	};
	if (destr == NULL) {
		print_error(("Null Destructor Error!"), NULL_DESTRUCTOR_ERROR);
	}
#ifdef DEBUG
	printf("Array deleting\n");
#endif

	for (size_t i = 0; i < this->size; i++) {
		destr(this->data[i]);
		this->data[i] = NULL;
	}

	free(this->data);
	this->size = 0;

	free(this);
	this = NULL;
}

void array_push(array *this, void *data) {
	this->data = realloc(this->data, (this->size + 1) * sizeof(void *));
	if (this->data == NULL) {
		print_error(("Array memory reallocation (push_back) error"), MEMORY_ALLOCATION_ERROR);
		return;
	}

	this->data[this->size] = data;
	this->size++;
}

void *array_get(array *this, size_t index) {
	return this->data[index];
}
