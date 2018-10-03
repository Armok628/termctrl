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
	current_zone=zone;
	clear_screen();
	draw_zone(zone);
	sgr(RESET);
	set_canon(false);
	set_cursor_visible(false);
	report_height=HEIGHT;
	while (!exit_req)
		advance(zone);
	clear_reports();
	set_canon(true);
	set_cursor_visible(true);
	move_cursor(0,report_height);
	sgr(RESET);
	clear_screen();
	move_cursor(0,0);
#define PUT_SIZEOF(type) printf("sizeof("#type"): %lu\n",sizeof(type))
	PUT_SIZEOF(struct entitytype);
	PUT_SIZEOF(struct entity);
	PUT_SIZEOF(struct tile);
	PUT_SIZEOF(struct worldtile);
	free_zone(zone);
	return 0;
}
