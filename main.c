#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/terminal.h"
#include "src/range.h"
#include "src/entity.h"
#include "src/tile.h"
#define WIDTH 80
#define HEIGHT 24
#define AREA (WIDTH*HEIGHT)
void draw_zone(struct tile *z)
{
	for (int i=0;i<AREA;i++) {
		move_cursor(i%WIDTH,i/WIDTH);
		draw_tile(&z[i]);
	}
}
void draw_pos(struct tile *z,int pos)
{
	move_cursor(pos%WIDTH,pos/WIDTH);
	draw_tile(&z[pos]);
}
bool legal_move(int from,int to)
{
	int dx=to%WIDTH-from%WIDTH;
	return -1<=dx&&dx<=1&&0<=to&&to<AREA;
}
void move_entity(struct tile *z,int from,int to)
{
	struct entity *e=z[from].e;
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
	if (!legal_move(from,to))
		to=from;
	move_entity(z,from,to);
	return to;
}
char grass_syms[]="\"';:.,`";
int n_grass_syms=sizeof(grass_syms)/sizeof(grass_syms[0])-1;
////////////////////////////////////////////////////////////////
struct tile zone[AREA];
int player_coords=(HEIGHT/2)*WIDTH+WIDTH/2;
struct entity player={.name="Player",.gr={FG_BLUE,true},.sym='@',.hp=1};
bool exit_req=false;
char key(void)
{
	char c=fgetc(stdin);
	exit_req=c==4;
	return c;
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
int main(int argc,char **argv)
{
	zone[player_coords].e=&player;
	srand(time(NULL));
	for (int i=0;i<argc;i++)
		printf("%s ",argv[i]);
	for (int i=0;i<AREA;i++) {
		zone[i].bg_gr[0]=FG_GREEN;
		zone[i].bg_gr[1]=rand()%2;
		zone[i].bg_sym=grass_syms[rand()%n_grass_syms];
		if (!(rand()%1000)) {
			zone[i].e=spawn(NULL);
			zone[i].e->name="Test";
			zone[i].e->gr[0]=FG_BLACK;
			zone[i].e->gr[1]=BOLD;
			zone[i].e->sym='&';
			zone[i].e->hp=1;
		}
	}
	clear_screen();
	draw_zone(zone);
	sgr(RESET);
	putchar('\n');
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
	return 0;
}
