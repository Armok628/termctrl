#include "tile.h"
const char *grass_syms="\"';:.,`";
const int n_grass_syms=sizeof(grass_syms)/sizeof(grass_syms[0])-1;
void draw_tile(struct tile *t)
{
	set_bg(BLACK);
	if (t->e)
		draw_entity(t->e);
	else if (t->c)
		draw_entity(t->c);
	else if (t->fg) {
		set_fg(t->fg_c);
		putchar(t->fg);
	} else if (t->bg) {
		set_fg(t->bg_c);
		putchar(t->bg);
	}
}
struct tile *new_zone(struct tile *z)
{
	if (!z)
		z=calloc(AREA,sizeof(struct tile));
	for (int i=0;i<AREA;i++) {
		z[i].bg_c=rand()%2?GREEN:LIGHT_GREEN;
		z[i].bg=grass_syms[rand()%n_grass_syms];
		/**/
		if (!(rand()%500)) {
			z[i].e=spawn(&monstertest);
		}
		/**/
	}
	/**/
	for (int i=0;i<AREA/96;i++)
		rand_room(z);
	fix_rooms(z);
	fix_gaps(z);
	/**/
	return z;
}
void free_zone(struct tile *z)
{
	for (int i=0;i<AREA;i++) {
		if (z[i].e)
			free_entity(z[i].e);
		if (z[i].c)
			free_entity(z[i].c);
	}
	free(z);
}
void draw_pos(struct tile *z,int pos)
{
	move_cursor(pos%WIDTH,pos/WIDTH);
	draw_tile(&z[pos]);
}
void draw_zone(struct tile *z)
{
	for (int i=0;i<AREA;i++)
		draw_pos(z,i);
}
bool legal_move(int from,int to)
{
	int dx=to%WIDTH-from%WIDTH;
	return -1<=dx&&dx<=1&&0<=to&&to<AREA;
}
