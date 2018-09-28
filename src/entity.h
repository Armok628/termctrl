#ifndef ENTITY_H
#define ENTITY_H
#include "terminal.h"
#include "range.h"
struct entitytype {
	char *name;
	char sym;
	char gr[2];
	range_t hp,res,agi,wis,str;
};
struct entity {
	struct entitytype *type;
	char *name;
	char sym;
	char gr[2];
	short hp,maxhp,res,agi,wis,str;
};

struct entity *spawn(struct entitytype *);
void draw_entity(struct entity *);
#endif
