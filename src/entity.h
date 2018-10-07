#ifndef ENTITY_H
#define ENTITY_H
#include "terminal.h"
#include "range.h"
#include "randword.h"
#include "color.h"
enum entityflags {
	NONE=0,
	NAMED=1<<0,
	OPENS_DOORS=1<<1,
};
struct entitytype {
	char *name;
	char sym;
	color_t color;
	char flags;
	range_t hp,res,agi,wis,str;
	struct entitytype *friends[8];
	struct entitytype *enemies[8];
};
struct entity {
	struct entitytype *type;
	char *name;
	char sym;
	color_t color;
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
