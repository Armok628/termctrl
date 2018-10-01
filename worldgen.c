#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "src/world.h"
#include "src/terminal.h"
int main(int argc,char **argv)
{
	int age=4,e_o=10,t_o=0;
	unsigned long seed=time(NULL);
	for (int i=1;i<argc;i++) {
		if (!strcmp(argv[i],"show_climates"))
			show_climates=true;
		sscanf(argv[i],"age=%d",&age);
		sscanf(argv[i],"e_o=%d",&e_o);
		sscanf(argv[i],"t_o=%d",&t_o);
		sscanf(argv[i],"seed=%lu",&seed);
	}
	srand(seed);
	clear_screen();
	struct worldtile *world=worldgen(age,e_o,t_o);
	draw_world(world);
	free(world);
	sgr(RESET);
	putchar('\n');
	printf("seed=%lu\n",seed);
	return 0;
}
