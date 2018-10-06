#ifndef ENTITY_H
#define ENTITY_H
#include "terminal.h"
#include "range.h"
#include "randword.h"
enum entityflags {
	NONE=0,
	NAMED=1<<0,
	OPENS_DOORS=1<<1,
};
struct entitytype {
	char *name;
	char sym;
	char gr[2];
	char flags;
	range_t hp,res,agi,wis,str;
};
struct entity {
	struct entitytype *type;
	char *name;
	char sym;
	char gr[2];
	char flags;
	short hp,maxhp,res,agi,wis,str;
};
extern struct entitytype playertest;
extern struct entitytype monstertest;

struct entity *transform(struct entity *,struct entitytype *);
struct entity *spawn(struct entitytype *);
void free_entity(struct entity *);
void draw_entity(struct entity *);
#endif
