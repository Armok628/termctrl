#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/terminal.h"
#include "src/range.h"
#include "src/entity.h"
#include "src/tile.h"
#include "src/input.h"
#include "src/world.h"
#include "src/advance.h"
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
	PUT_SIZEOF(struct worldtile);
	free_zone(zone);
	return 0;
}
