#ifndef TILE_H
#define TILE_H
#include "terminal.h"
#include "entity.h"
struct tile {
	struct entity *e,*c;
	char fg_sym,bg_sym;
	char fg_mode[2],bg_mode[2];
};

void draw_tile(struct tile *);
#endif
