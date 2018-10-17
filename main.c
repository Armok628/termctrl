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
#include "src/room.h"
#include "src/zone.h"
int main(/*int argc,char **argv*/)
{
	srand(time(NULL));
	struct tile *zone=zonegen(NULL,0);
	current_zone=zone;
	player_pos=rand()%Z_AREA;
#ifdef SCROLL
	report_height=G_HEIGHT;
	scroll_zone(player_pos);
#else
	report_height=Z_HEIGHT;
#endif
	zone[player_pos].e=spawn(&playertest);
	clear_screen();
	draw_zone(zone);
	sgr(RESET);
	set_canon(false);
	set_cursor_visible(false);
	while (!exit_req)
		advance(current_zone);
	free_zone(zone);
	set_canon(true);
	set_cursor_visible(true);
	sgr(RESET);
	clear_screen();
	move_cursor(0,0);
#define PUT_SIZEOF(type) printf("sizeof("#type"): %lu\n",sizeof(type))
	PUT_SIZEOF(struct entitytype);
	PUT_SIZEOF(struct entity);
	PUT_SIZEOF(struct tile);
	PUT_SIZEOF(struct worldtile);
	return 0;
}
