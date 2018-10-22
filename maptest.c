#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "src/map.h"
#include "src/terminal.h"
int main(int argc,char **argv)
{
	int age=4,e_o=0,t_o=0;
	float e_f=1.0,t_f=1.0;
	unsigned long seed=time(NULL);
	for (int i=1;i<argc;i++) {
		if (!strcmp(argv[i],"show_climates"))
			show_climates=true;
		sscanf(argv[i],"seed=%lu",&seed);
		sscanf(argv[i],"age=%d",&age);
		sscanf(argv[i],"e_o=%d",&e_o);
		sscanf(argv[i],"t_o=%d",&t_o);
		sscanf(argv[i],"e_f=%f",&e_f);
		sscanf(argv[i],"t_f=%f",&t_f);
	}
	srand(seed);
	clear_screen();
	struct worldtile *world=worldgen(age,e_o,t_o,e_f,t_f);
	world_pos=rand_land(world);
#ifdef SCROLL
	scroll_map(world_pos);
#endif
	set_canon(false);
	set_cursor_visible(false);
	open_map(world);
	set_canon(true);
	set_cursor_visible(true);
	free(world);
	sgr(RESET);
	move_cursor(0,report_height);
	return 0;
}
