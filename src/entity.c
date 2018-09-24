#include "entity.h"
struct entity *spawn(struct entitytype *t)
{
	struct entity *e=malloc(sizeof(struct entity));
	e->sym=t->sym;
	e->name=t->name;
	e->mode=t->mode;
	e->maxhp=rrand(t->hp);
	e->hp=e->maxhp;
	e->res=rrand(t->res);
	e->agi=rrand(t->agi);
	e->wis=rrand(t->wis);
	e->str=rrand(t->str);
	return e;
}
