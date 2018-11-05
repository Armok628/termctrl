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
static inline void incr_size(struct faction *f)
{
	f->size++;
}
#define MAX_FACTIONS 64
extern struct faction *factions[MAX_FACTIONS];
extern int num_factions;

int spread_influence(struct worldtile *,int);
void decr_size(struct faction *);
void spread_faction(struct worldtile *,struct faction *);
void spread_all_factions(struct worldtile *);
void recolor_faction(struct faction *);
struct faction *random_faction(void);
bool in_territory(struct worldtile *,int);
void place_rebellion(struct worldtile *,int,struct faction *);
void random_rebellion(struct worldtile *,struct faction *);
void form_colony(struct worldtile *,struct faction *);
#endif
