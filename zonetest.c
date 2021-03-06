#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/util/input.h"
#include "src/util/report.h"
#include "src/zone/path.h"
#include "src/zone/room.h"
#include "src/zone/zone.h"
struct creature player={
	.name="Player",
	.sym='@',
	.color=LIGHT_BLUE,
};
struct entity player_ent={
	.type=CREATURE,
	.ptr={.c=&player},
	.next_entity=NULL,
};
int main()//(int argc,char **argv)
{
	srand(time(NULL));
	init_rogue();
	struct tile *z=new_zone();
	for (int i=0;i<5;i++)
		random_room(z);
	place_doors(z);
	int pc=spawn(z,&player_ent);
	scroll_zone(pc);
	draw_zone(z);
	report_height=TERM_HEIGHT;
	int t=rand()%ZONE_AREA;
	while (z[t].bg!='#')
		t=rand()%ZONE_AREA;
	while (!exit_req) {
		char c=key();
		int o=input_offset(c,ZONE_WIDTH);
		if (o)
			t=target(z,pc);
		c=go_to(z,pc,t);
		o=input_offset(c,ZONE_WIDTH);
		pc=move_entity(z,pc,pc+o);
		scroll_zone(pc);
		draw_zone(z);
	}
	free(z);
	endwin();
	return 0;
}
