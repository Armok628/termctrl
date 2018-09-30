#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/terminal.h"
#include "src/range.h"
#include "src/entity.h"
#include "src/tile.h"
#include "src/input.h"
void move_entity(struct tile *z,int from,int to)
{
	if (from==to)
		return;
	struct entity *e=z[from].e;
	if (z[to].e)
		free_entity(z[to].e);
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
int player_coords;
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
int main(/*int argc,char **argv*/)
{
	srand(time(NULL));
	struct tile *zone=new_zone(NULL);
	player_coords=rand()%AREA;
	zone[player_coords].e=spawn(&playertest);
	clear_screen();
	draw_zone(zone);
	sgr(RESET);
	set_canon(false);
	set_cursor_visible(false);
	while (!exit_req) {
		move_cursor(0,HEIGHT);
		clear_line();
		printf("%s ",zone[player_coords].e->name);
		draw_entity(zone[player_coords].e);
		advance(zone);
	}
	set_canon(true);
	set_cursor_visible(true);
	move_cursor(0,HEIGHT);
	sgr(RESET);
#define PUT_SIZEOF(type) printf("sizeof("#type"): %lu\n",sizeof(type))
	PUT_SIZEOF(struct entitytype);
	PUT_SIZEOF(struct entity);
	PUT_SIZEOF(struct tile);
	free_zone(zone);
	return 0;
}
