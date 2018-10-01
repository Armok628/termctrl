#ifndef WORLD_H
#define WORLD_H
#include <stdlib.h>
#include <stdbool.h>
#include "terminal.h"
#define W_WIDTH 80
#define W_HEIGHT 24
#define W_AREA (W_WIDTH*W_HEIGHT)
struct worldtile {
	short elev,temp;
};
extern bool show_climates;

short avg_elev(short *,int);
void erode(struct worldtile *);
struct worldtile *worldgen(int,int,int);
void draw_worldtile(struct worldtile);
void draw_world(struct worldtile *);
#endif
