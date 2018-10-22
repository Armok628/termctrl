#ifndef WORLD_H
#define WORLD_H
#include <stdlib.h>
#include <stdbool.h>
#include "terminal.h"
#include "color.h"
#include "input.h"
#ifndef W_WIDTH
#define W_WIDTH 80
#endif
#ifndef W_HEIGHT
#define W_HEIGHT 24
#endif
#define W_AREA (W_WIDTH*W_HEIGHT)
#define TERRAIN(type) COLD_##type, type, HOT_##type
enum terrain {
	TERRAIN(NO_TERRAIN),
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
enum dir {
	SOUTHWEST=1,
	SOUTH=2,
	SOUTHEAST=3,
	WEST=4,
	CENTER=5,
	EAST=6,
	NORTHWEST=7,
	NORTH=8,
	NORTHEAST=9,
};
struct worldtile {
	short elev,temp;
	struct tile *zone;
	enum dir river;
	bool town;
};
extern bool show_climates;

struct worldtile *worldgen(int,int,int,float,float);
enum terrain terrain_type(struct worldtile *);
void draw_worldtile(struct worldtile *);
void draw_whole_world(struct worldtile *);
int rand_land(struct worldtile *);
#endif
