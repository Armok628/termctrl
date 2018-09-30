#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/world.h"
#include "src/terminal.h"
int main(int argc,char **argv)
{
	int erosion=4,offset=10;
	unsigned long seed=time(NULL);
	for (int i=1;i<argc;i++) {
		sscanf(argv[i],"erosion=%d",&erosion);
		sscanf(argv[i],"offset=%d",&offset);
		sscanf(argv[i],"seed=%lu",&seed);
	}
	srand(seed);
	clear_screen();
	struct worldtile *world=worldgen(erosion,offset);
	draw_world(world);
	free(world);
	sgr(RESET);
	putchar('\n');
	printf("seed=%lu\n",seed);
	return 0;
}
