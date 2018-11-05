#ifndef FACTION_H
#define FACTION_H
#include "color.h"
#include "world.h"
#include "map.h"
struct faction {
	char *name;
	color_t color;
};

void spread_faction(struct worldtile *,struct faction *);
void spread_all_factions(struct worldtile *);
void recolor_faction(struct faction *);
struct faction *random_faction(void);
#endif
