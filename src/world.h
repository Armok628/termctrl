#ifndef WORLD_H
#define WORLD_H
#include "tile.h"
#include "terminal.h"
struct worldtile {
	short elev,temp;
};

void erode(struct worldtile *);
struct worldtile *worldgen(void);
void draw_worldtile(struct worldtile);
void draw_world(struct worldtile *);
#endif
