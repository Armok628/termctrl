#ifndef WORLD_H
#define WORLD_H
#include <stdlib.h>
#include <stdbool.h>
#include "../util/colors.h"
#include "../util/input.h"
#include "../util/erode.h"
#ifndef WORLD_WIDTH
#define WORLD_WIDTH 80
#endif
#ifndef WORLD_HEIGHT
#define WORLD_HEIGHT 24
#endif
#define WORLD_AREA (WORLD_WIDTH*WORLD_HEIGHT)
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
#define TOWN_POP_CAP 1000
struct worldtile {
	short elev,temp;
	struct tile *zone;
	struct faction *faction;
	enum dir river;
	bool town;
	short pop;
};
extern bool show_climates;
#include "faction.h"

struct worldtile *worldgen(int,int,int,float,float);
enum terrain terrain_type(struct worldtile *);
void draw_worldtile(struct worldtile *);
void draw_whole_world(struct worldtile *);
int rand_loc(struct worldtile *,bool (*)(struct worldtile *,int));
bool is_land(struct worldtile *,int);
#endif
