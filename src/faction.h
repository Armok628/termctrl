#ifndef FACTION_H
#define FACTION_H
#include "color.h"
#include "world.h"
#include "map.h"
struct faction {
	char *name;
	color_t color;
};
void spread_factions(struct worldtile *);
#endif
