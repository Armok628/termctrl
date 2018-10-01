#include "advance.h"
int player_coords;
void move_entity(struct tile *z,int from,int to)
{
	if (from==to)
		return;
	struct entity *e=z[from].e;
	if (z[to].e) {
		z[to].c=z[to].e;
		/**/z[to].c->hp=0;
	}
	z[from].e=NULL;
	draw_pos(z,from);
	z[to].e=e;
	draw_pos(z,to);
}
int handle_move(struct tile *z,int from,char c)
{
	int to=from+input_offset(c);
	if (!legal_move(from,to)||z[to].fg_sym)
		to=from;
	move_entity(z,from,to);
	return to;
}
void take_turn(struct tile *z,int pos)
{
	if (!z[pos].e)
		return;
	if (pos==player_coords) {
		player_coords=handle_move(z,pos,key());
	} else
		handle_move(z,pos,'0'+rand()%('9'-'0'-1));
}
void advance(struct tile *z)
{
	struct entity *e[AREA];
	for (int i=0;i<AREA;i++)
		e[i]=z[i].e;
	for (int i=0;i<AREA;i++)
		if (z[i].e==e[i])
			take_turn(z,i);
}
