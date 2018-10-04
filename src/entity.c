#include "entity.h"
struct entitytype monstertest={
	.name="Monster",
	.sym='&',
	.gr={FG_BLACK,BOLD},
	.flags=NONE,
	.hp={50,150},
	.res={5,15},
	.agi={5,15},
	.wis={5,15},
	.str={5,15},
};
struct entitytype playertest={
	.name="Player",
	.sym='@',
	.gr={FG_BLUE,BOLD},
	.flags=NAMED|OPENS_DOORS,
	.hp={50,150},
	.res={5,15},
	.agi={5,15},
	.wis={5,15},
	.str={5,15},
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
