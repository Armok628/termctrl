#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "src/map.h"
#include "src/terminal.h"
int main(int argc,char **argv)
{
	int age=DEF_AGE,e_o=DEF_E_O,t_o=DEF_T_O;
	float e_f=DEF_E_F,t_f=DEF_T_F;
	unsigned long seed=time(NULL);
	for (int i=1;i<argc;i++) {
		sscanf(argv[i],"seed=%lu",&seed);
		sscanf(argv[i],"age=%d",&age);
		sscanf(argv[i],"e_o=%d",&e_o);
		sscanf(argv[i],"t_o=%d",&t_o);
		sscanf(argv[i],"e_f=%f",&e_f);
		sscanf(argv[i],"t_f=%f",&t_f);
	}
	srand(seed);
	clear_screen();
	move_cursor(0,0);
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
	for (int i=0;i<num_factions;i++) {
		free(factions[i]->name);
		free(factions[i]);
	}
	sgr(RESET);
	move_cursor(0,report_height);
	return 0;
}
