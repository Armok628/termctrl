#include "entity.h"
struct entity *spawn(struct entitytype *t)
{
	struct entity *e=malloc(sizeof(struct entity));
	if (t) {
		e->sym=t->sym;
		e->name=t->name;
		e->gr[0]=t->gr[0];
		e->gr[1]=t->gr[1];
		e->maxhp=rrand(t->hp);
		e->hp=e->maxhp;
		e->res=rrand(t->res);
		e->agi=rrand(t->agi);
		e->wis=rrand(t->wis);
		e->str=rrand(t->str);
	}
	return e;
}
