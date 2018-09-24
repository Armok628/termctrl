#include <stdio.h>
#include <stdlib.h>
#include "src/terminal.h"
#include "src/range.h"
#include "src/entity.h"
#include "src/tile.h"
#define WIDTH 80
#define HEIGHT 24
#define AREA (WIDTH*HEIGHT)
struct tile zone[AREA];
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
	for (int i=0;i<AREA;i++) {
		zone[i].bg_mode=grass_modes[rand()%2];
		zone[i].bg_sym=grass_syms[rand()%7];
	}
	clear_screen();
	draw_zone(zone);
	sgr(RESET);
	putchar('\n');
#define PUT_SIZEOF(type) printf("sizeof("#type"): %lu\n",sizeof(type))
	PUT_SIZEOF(struct entitytype);
	PUT_SIZEOF(struct entity);
	PUT_SIZEOF(struct tile);
	return 0;
}
