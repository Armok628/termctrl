#include "entity.h"
struct entitytype monstertest={
	.name="Monster",
	.sym='&',
	.gr={FG_BLACK,BOLD},
	.hp={100,100},
	.res={10,10},
	.agi={10,10},
	.wis={10,10},
	.str={10,10},
	.flags=NONE,
};
struct entitytype playertest={
	.name="Player",
	.sym='@',
	.gr={FG_BLUE,BOLD},
	.hp={100,100},
	.res={10,10},
	.agi={10,10},
	.wis={10,10},
	.str={10,10},
	.flags=NAMED,
};
struct entity *spawn(struct entitytype *t)
{
	struct entity *e=malloc(sizeof(struct entity));
	if (t) {
		e->sym=t->sym;
		e->type=t;
		e->flags=t->flags;
		if (e->flags&NAMED) {
			e->name=random_word(4+rand()%4);
			e->name[0]+='A'-'a';
		} else
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
void free_entity(struct entity *e)
{
	if (e->flags&NAMED)
		free(e->name);
	free(e);
}
void draw_entity(struct entity *e)
{
	sgr(RESET);
	sgr(e->hp>0?BG_BLACK:BG_RED);
	sgr(e->gr[0]);
	sgr(e->gr[1]);
	putchar(e->sym);
}
