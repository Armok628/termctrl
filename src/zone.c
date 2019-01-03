#include "zone.h"
int zone_xscroll=0,zone_yscroll=0;
void scroll_zone(int i)
{
	zone_xscroll=TERM_WIDTH/2-i%ZONE_WIDTH;
	zone_yscroll=TERM_HEIGHT/2-i/ZONE_WIDTH;
}
void draw_entity(struct entity *e)
{
	struct creature *c=e->ptr.c;
	struct item *i=e->ptr.i;
	switch (e->type) {
	case CREATURE:
		draw(c->sym,c->color,/**/BLACK/**/);
		break;
	case ITEM:
		draw(i->sym,i->color,BLACK);
		break;
	}
}
void draw_tile(struct tile *t)
{
	if (t->fg)
		draw(t->fg,t->fg_c,BLACK);
	else if (t->e)
		draw_entity(t->e);
	else
		draw(t->bg,t->bg_c,BLACK);
}
void draw_zone_pos(struct tile *z,int p)
{
	int x=p%ZONE_WIDTH+zone_xscroll;
	int y=p/ZONE_WIDTH+zone_yscroll;
	if (0<=x&&x<ZONE_WIDTH&&0<=y&&y<ZONE_HEIGHT) {
		next_draw(x,y);
		draw_tile(&z[p]);
	}
}
void draw_zone(struct tile *z)
{
	for (int x=0;x<TERM_WIDTH;x++)
	for (int y=0;y<TERM_HEIGHT;y++) {
		int x2=x-zone_xscroll,y2=y-zone_yscroll;
		next_draw(x,y);
		if (0>x2||x2>=ZONE_WIDTH||0>y2||y2>=ZONE_HEIGHT)
			draw(' ',RESET,RESET);
		else
			draw_tile(&z[x2+y2*ZONE_WIDTH]);
	}
}
int rand_tile(struct tile *z,bool (*f)(struct tile *))
{
	int choices[ZONE_AREA];
	int n_choices=0;
	for (int i=0;i<ZONE_AREA;i++)
		if (f(&z[i]))
			choices[n_choices++]=i;
	return n_choices?choices[rand()%n_choices]:-1;
}
bool empty_tile(struct tile *t)
{
	return !t->fg&&!t->e;
}
int spawn(struct tile *z,struct entity *e)
{
	int n=rand_tile(z,empty_tile);
	z[n].e=e;
	return n;
}
struct tile *new_zone(void)
{
	static const char grass[]=",.;:'\"`";
	static const int n_grass=sizeof(grass)-1;
	struct tile *z=malloc(ZONE_AREA*sizeof(struct tile));
	for (int i=0;i<ZONE_AREA;i++) {
		z[i].fg='\0';
		z[i].e=NULL;
		z[i].bg=grass[rand()%n_grass];
		z[i].bg_c=rand()&1?GREEN:LIGHT_GREEN;
	}
	return z;
}
