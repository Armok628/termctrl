#include <stdio.h>
#include <stdlib.h>
#include "src/terminal.h"
struct entity {
	char sym;
	gr_t mode;
};
struct tile {
	struct entity *e;
	char fg_sym,bg_sym;
	gr_t fg_mode,bg_mode;
};
#define WIDTH 80
#define HEIGHT 24
#define AREA (WIDTH*HEIGHT)
struct tile zone[AREA];
void draw_tile(struct tile *t)
{
	sgr(RESET);
	if (t->e) {
		mode(t->e->mode);
		putchar(t->e->sym);
	} else if (t->fg_sym) {
		mode(t->fg_mode);
		putchar(t->fg_sym);
	} else if (t->bg_sym) {
		mode(t->bg_mode);
		putchar(t->bg_sym);
	}
}
void draw_zone(struct tile *z)
{
	for (int i=0;i<AREA;i++) {
		move_cursor(i%WIDTH,i/WIDTH);
		draw_tile(&z[i]);
	}
}
gr_t grass_modes[2]={{.fg=FG_GREEN,.bg=BG_BLACK,.b=true},{.fg=FG_GREEN,.bg=BG_BLACK,.b=false}};
char grass_syms[8]="\"';:.,`";
int main(int argc,char **argv)
{
	gr_t grass_mode={FG_GREEN,BG_BLACK,0};
	for (int i=0;i<AREA;i++) {
		zone[i].bg_mode=grass_modes[rand()%2];
		zone[i].bg_sym=grass_syms[rand()%7];
	}
	clear_screen();
	draw_zone(zone);
	sgr(RESET);
	putchar('\n');
	return 0;
}
