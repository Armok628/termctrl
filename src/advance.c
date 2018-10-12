#include "advance.h"
int player_pos=-1;
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
	if (z==current_zone) {
		if (from==player_pos) {
			player_pos=to;
#ifdef SCROLL_ZONE
			x_offset=G_WIDTH/2-to%Z_WIDTH;
			y_offset=G_HEIGHT/2-to/Z_WIDTH;
			clear_screen();
			draw_zone(current_zone);
#endif
		}
		draw_pos(z,from);
		draw_pos(z,to);
	}
}
void handle_move(struct tile *z,int from,char c)
{
	int to=from+input_offset(c);
	if (!legal_move(from,to)||to==from)
		return;
	if (z[to].fg) {
		if (~z[from].e->flags&OPENS_DOORS)
			return;
		if (z[to].fg=='+'&&z[to].bg=='-') {
			z[to].fg='\0';
			if (z==current_zone)
				draw_pos(z,to);
		}
		return;
	}
	if (z[to].e&&!attack(z,from,to))
			return;
	move_entity(z,from,to);
}
bool take_turn(struct tile *z,int pos)
{ // Returns true if turn is finished
	if (z==current_zone&&pos==player_pos) {
		update_stats(z,player_pos);
		char c=key();
		clear_reports();
		switch (c) {
		case 0:
			command(z,pos);
			return false;
		case 'R':
			clear_screen();
			draw_zone(z);
			return false;
		case 'c':
			pos+=input_offset(key());
			if (z[pos].bg=='-'&&z[pos].bg_c==BROWN&&z[pos].fg_c==BROWN) {
				z[pos].fg='+';
				draw_pos(z,pos);
			}
			return true;
		}
		if (z[pos].e)
			handle_move(z,pos,c);
	} else if (z[pos].e)
		handle_move(z,pos,think(z,pos));
	return true;
}
void advance(struct tile *z)
{
	struct entity *e[AREA];
	for (int i=0;i<AREA;i++)
		e[i]=z[i].e;
	for (int i=0;i<AREA;i++)
		if (z[i].e==e[i])
			while (!take_turn(z,i));
}
