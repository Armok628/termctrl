#ifndef WORLD_H
#define WORLD_H
#include "tile.h"
#include "terminal.h"
struct worldtile {
	short elev,temp;
};

short avg_elev(short *,int);
void erode(struct worldtile *);
struct worldtile *worldgen(int,int);
void draw_worldtile(struct worldtile);
void draw_world(struct worldtile *);
#endif
