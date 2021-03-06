#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "src/world/map.h"
#include "src/util/colors.h"
#ifndef DEF_AGE
#define DEF_AGE 4
#endif
#ifndef DEF_E_O
#define DEF_E_O 0
#endif
#ifndef DEF_T_O
#define DEF_T_O 0
#endif
#ifndef DEF_E_F
#define DEF_E_F 1.0
#endif
#ifndef DEF_T_F
#define DEF_T_F 1.0
#endif
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
	init_rogue();
	struct worldtile *world=worldgen(age,e_o,t_o,e_f,t_f);
	world_pos=rand_loc(world,&is_land);
	scroll_map(world_pos);
	open_map(world);
	free(world);
	for (int i=0;i<num_factions;i++) {
		free(factions[i]->name);
		free(factions[i]);
	}
	endwin();
	return 0;
}
