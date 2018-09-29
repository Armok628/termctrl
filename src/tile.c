#include "tile.h"
const char *grass_syms="\"';:.,`";
const int n_grass_syms=sizeof(grass_syms)/sizeof(grass_syms[0])-1;
static inline void sgr2(char *arr)
{
	sgr(arr[0]);
	if (arr[1])
		sgr(arr[1]);
}
void draw_tile(struct tile *t)
{
	sgr(RESET);
	sgr(BG_BLACK);
	if (t->e) {
		draw_entity(t->e);
	} else if (t->fg_sym) {
		sgr2(t->fg_gr);
		putchar(t->fg_sym);
	} else if (t->bg_sym) {
		sgr2(t->bg_gr);
		putchar(t->bg_sym);
	}
}
struct tile *new_zone(struct tile *z)
{
	if (!z)
		z=calloc(AREA,sizeof(struct tile));
	for (int i=0;i<AREA;i++) {
		z[i].bg_gr[0]=FG_GREEN;
		z[i].bg_gr[1]=rand()%2;
		z[i].bg_sym=grass_syms[rand()%n_grass_syms];
		if (!(rand()%1000)) {
			z[i].e=spawn(&monstertest);
		} else if (!(rand()%500)) {
			z[i].fg_sym='#';
			z[i].fg_gr[0]=FG_GRAY;
			z[i].fg_gr[1]=NO_BOLD;
		}
	}
	return z;
}
void free_zone(struct tile *z)
{
	for (int i=0;i<AREA;i++)
		if (z[i].e)
			free_entity(z[i].e);
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
