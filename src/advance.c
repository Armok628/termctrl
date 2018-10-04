#include "advance.h"
int player_coords=-1;
struct tile *current_zone=NULL;
void kill(struct tile *z,int pos)
{
	z[pos].e->hp=0;
	if (z[pos].c)
		free_entity(z[pos].c);
	z[pos].c=z[pos].e;
	z[pos].e=NULL;
}
void move_entity(struct tile *z,int from,int to)
{
	if (to==from)
		return;
	z[to].e=z[from].e;
	z[from].e=NULL;
	draw_pos(z,from);
	draw_pos(z,to);
}
int handle_move(struct tile *z,int from,char c)
{ // Returns entity's new position
	int to=from+input_offset(c);
	if (!legal_move(from,to)||to==from)
		return from;
	if (z[to].fg_sym) {
		if (~z[from].e->flags&OPENS_DOORS)
			return from;
		if (z[to].fg_sym=='+'&&z[to].bg_sym=='-') {
			z[to].fg_sym='\0';
			draw_pos(z,to);
		}
		return from;
	}
	if (z[to].e) {
		report("e s e",z[from].e,"kills",z[to].e);
		kill(z,to);
	}
	move_entity(z,from,to);
	return to;
}
void take_turn(struct tile *z,int pos)
{
	if (z==current_zone&&pos==player_coords) {
		update_stats(z[player_coords].e);
		char c=key();
		clear_reports();
		player_coords=handle_move(z,pos,c);
		update_stats(z[player_coords].e);
	} else
		handle_move(z,pos,'0'+rand()%('9'-'0'+1));
}
void advance(struct tile *z)
{
	struct entity *e[AREA];
	for (int i=0;i<AREA;i++)
		e[i]=z[i].e;
	for (int i=0;i<AREA;i++)
		if (e[i]&&z[i].e==e[i])
			take_turn(z,i);
}
