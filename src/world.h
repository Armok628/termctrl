#ifndef WORLD_H
#define WORLD_H
#include <stdlib.h>
#include <stdbool.h>
#include "terminal.h"
#include "color.h"
#ifndef W_WIDTH
#define W_WIDTH 80
#endif
#ifndef W_HEIGHT
#define W_HEIGHT 24
#endif
#define W_AREA (W_WIDTH*W_HEIGHT)
#define TERRAIN(type) COLD_##type, type, HOT_##type
enum terrain {
	TERRAIN(VOID),
	TERRAIN(DEEP_SEA),
	TERRAIN(SHALLOW_SEA),
	TERRAIN(BEACH),
	TERRAIN(MEADOW),
	TERRAIN(FIELD),
	TERRAIN(FOREST),
	TERRAIN(LOW_MOUNTAIN),
	TERRAIN(HIGH_MOUNTAIN),
	TERRAIN(SUMMIT),
	TERRAIN(HIGH_SUMMIT),
};
struct worldtile {
	short elev,temp;
};
extern bool show_climates;

struct worldtile *worldgen(int,int,int,float,float);
enum terrain terrain_type(struct worldtile);
void draw_terrain(enum terrain);
void draw_world(struct worldtile *);
#endif
