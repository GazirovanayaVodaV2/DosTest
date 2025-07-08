#include "assets.h"

atlas *load_assets(byte size) {
	atlas *this = new_atlas(size);

	size_t len = sizeof(assets) / sizeof(char *);
	for (size_t i = 0; i < len; i++) {
		atlas_push(this, new_image(assets[i]));
	}

	return this;
}