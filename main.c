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
struct tile zone[AREA];
void draw_zone(struct tile *z)
{
	for (int i=0;i<AREA;i++) {
		move_cursor(i%WIDTH,i/WIDTH);
		draw_tile(&z[i]);
	}
}
char grass_syms[]="\"';:.,`";
int n_grass_syms=sizeof(grass_syms)/sizeof(grass_syms[0])-1;
int main(int argc,char **argv)
{
	srand(time(NULL));
	for (int i=0;i<argc;i++)
		printf("%s ",argv[i]);
	for (int i=0;i<AREA;i++) {
		zone[i].bg_mode=rand()%2?FG_GREEN:AIX_FG_GREEN;
		zone[i].bg_sym=grass_syms[rand()%n_grass_syms];
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
