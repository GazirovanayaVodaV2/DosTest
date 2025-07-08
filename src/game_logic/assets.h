#ifndef ASSETS_H
#define ASSETS_H

#include "../graphics/graph.h"
#include <stddef.h>

static const char *assets[] = {
		"assets/bricks.vga",
		"assets/exit.vga"};

atlas *load_assets(byte size);

#endif