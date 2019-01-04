#include "zone.h"
int zone_xscroll=0,zone_yscroll=0;
void scroll_zone(int i)
{
	if (ZONE_WIDTH>TERM_WIDTH||ZONE_HEIGHT>TERM_HEIGHT) {
		zone_xscroll=TERM_WIDTH/2-i%ZONE_WIDTH;
		zone_yscroll=TERM_HEIGHT/2-i/ZONE_WIDTH;
	}
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
	else if (t->next_entity)
		draw_entity(t->next_entity);
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
	return !t->fg&&!t->next_entity;
}
int spawn(struct tile *z,struct entity *e)
{
	int n=rand_tile(z,empty_tile);
	z[n].next_entity=e;
	return n;
}
struct tile *new_zone(void)
{
	static const char grass[]=",.;:'\"`";
	static const int n_grass=sizeof(grass)-1;
	struct tile *z=malloc(ZONE_AREA*sizeof(struct tile));
	for (int i=0;i<ZONE_AREA;i++) {
		z[i].fg='\0';
		z[i].next_entity=NULL;
		z[i].bg=grass[rand()%n_grass];
		z[i].bg_c=rand()&1?GREEN:LIGHT_GREEN;
	}
	return z;
}
bool in_bounds(int from,int to)
{
	int dx=to%ZONE_WIDTH-from%ZONE_WIDTH;
	return -1<=dx&&dx<=1&&0<=to&&to<ZONE_AREA;
}
int move_entity(struct tile *z,int from,int to)
{
	if (!in_bounds(from,to))
		return from;
	if (!z[to].fg) {
		// TODO: Add way to move specific entity from tile
		struct entity *e=z[from].next_entity;
		z[from].next_entity=e->next_entity;
		e->next_entity=z[to].next_entity;
		z[to].next_entity=e;
		return to;
	} else if (z[to].fg=='+'&&z[to].fg_c==BROWN) {
		z[to].fg='\0';
		return from;
	}
	return from;
}
int target(struct tile *z,int start)
{
	char c='\0';
	int i=start;
	do {
		scroll_zone(i);
		draw_zone(z);
		int x=i%ZONE_WIDTH+zone_xscroll;
		int y=i/ZONE_WIDTH+zone_yscroll;
		next_draw(x,y);
		draw('X',RED,BLACK);
		c=key();
		int o=input_offset(c,ZONE_WIDTH);
		if (o&&in_bounds(i,i+o))
			i+=o;
	} while (c!='\n'&&!exit_req);
	scroll_zone(start);
	draw_zone(z);
	return i;
}
