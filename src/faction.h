#ifndef FACTION_H
#define FACTION_H
#include "color.h"
#include "world.h"
#include "map.h"
struct faction {
	char *name;
	color_t color;
	int size;
	int stagnation;
};
#ifndef MAX_FACTIONS
#define MAX_FACTIONS 7
#endif
extern struct faction *factions[MAX_FACTIONS];
extern int num_factions;

void destroy_faction(struct faction *);
void spread_faction(struct worldtile *,struct faction *);
void cull_dead_factions(void);
void advance_world(struct worldtile *);
struct faction *random_faction(void);
void random_rebellion(struct worldtile *,struct faction *);
void annex(struct worldtile *,struct faction *,struct faction *);
void form_colony(struct worldtile *,struct faction *);
#endif
