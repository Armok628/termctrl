#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/terminal.h"
#include "src/range.h"
#include "src/entity.h"
#include "src/tile.h"
bool legal_move(int from,int to)
{
	int dx=to%WIDTH-from%WIDTH;
	return -1<=dx&&dx<=1&&0<=to&&to<AREA;
}
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
int handle_move(struct tile *z,int from,char input)
{
	static int offsets[10]={0,
		WIDTH-1,WIDTH,WIDTH+1,
		-1,0,1,
		-WIDTH-1,-WIDTH,-WIDTH+1};
	int to=from;
	if ('0'<=input&&input<='9')
		to=from+offsets[input-'0'];
	if (!legal_move(from,to)||z[to].fg_sym)
		to=from;
	move_entity(z,from,to);
	return to;
}
bool exit_req=false;
char key(void)
{
	char c=fgetc(stdin);
	exit_req=c==4;
	return c;
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
