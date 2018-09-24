#ifndef TILE_H
#define TILE_H
#include "terminal.h"
#include "entity.h"
struct tile {
	struct entity *e,*c;
	char fg_sym,bg_sym;
	gr_t fg_mode,bg_mode;
};

void draw_tile(struct tile *);
#endif
