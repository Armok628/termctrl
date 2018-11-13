#include "entity.h"
struct entitytype monstertest={
	.name="Monster",
	.sym='&',
	.color=DARK_GRAY,
	.flags=NONE,
	.hp={50,150},
	.res={5,15},
	.agi={5,15},
	.wis={5,15},
	.str={5,15},
	.friends={&monstertest},
	.enemies={&playertest},
};
struct entitytype playertest={
	.name="Player",
	.sym='@',
	.color=LIGHT_BLUE,
	.flags=NAMED|OPENS_DOORS,
	.hp={50,150},
	.res={5,15},
	.agi={5,15},
	.wis={5,15},
	.str={5,15},
	.enemies={&monstertest},
};
struct entity *transform(struct entity *e,struct entitytype *t)
{
	e->type=t;
	if (e->flags&NAMED) {
		free(e->name);
	}
	e->flags=t->flags;
	if (t->flags&NAMED)
		e->name=capitalize(random_word(4+rand()%4));
	else
		e->name=t->name;
	e->sym=t->sym;
	e->color=t->color;
	e->maxhp=rrand(t->hp);
	e->hp=e->maxhp;
	e->res=rrand(t->res);
	e->agi=rrand(t->agi);
	e->wis=rrand(t->wis);
	e->str=rrand(t->str);
	return e;
}
struct entity *spawn(struct entitytype *t)
{
	struct entity *e=calloc(1,sizeof(struct entity));
	if (t)
		transform(e,t);
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
	set_fg(e->color);
	set_bg(e->hp>0?BLACK:RED);
	putchar(e->sym);
}
