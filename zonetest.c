#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/input.h"
#include "src/zone.h"
struct creature player={
	.name="Player",
	.sym='@',
	.color=LIGHT_BLUE,
};
struct entity player_ent={
	.type=CREATURE,
	.ptr={.c=&player},
};
int main(int argc,char **argv)
{
	srand(time(NULL));
	struct tile *z=new_zone();
	clear_screen();
	int pc=spawn(z,&player_ent);
	scroll_zone(pc);
	draw_zone(z);
	set_canon(false);
	set_cursor_visible(false);
	while (!exit_req) {
		char c=key();
		int o=input_offset(c,ZONE_WIDTH);
		z[pc].e=NULL;
		pc+=o;
		z[pc].e=&player_ent;
		scroll_zone(pc);
		draw_zone(z);
	}
	set_canon(true);
	set_cursor_visible(true);
	sgr(RESET);
	putchar('\n');
	free(z);
	return 0;
}
