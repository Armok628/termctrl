#ifndef ENTITY_H
#define ENTITY_H
#include "terminal.h"
#include "range.h"
struct entitytype {
	char *name;
	char sym;
	gr_t mode;
	range_t hp,res,agi,wis,str;
};
struct entity {
	struct entitytype *type;
	char *name;
	char sym;
	gr_t mode;
	short hp,maxhp,res,agi,wis,str;
};

struct entity *spawn(struct entitytype *);
#endif