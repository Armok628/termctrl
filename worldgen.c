#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/world.h"
#include "src/terminal.h"
int main(int argc,char **argv)
{
	int erosion=4,offset=10;
	if (argc>1)
		sscanf(argv[1],"%d",&erosion);
	if (argc>2)
		sscanf(argv[2],"%d",&offset);
	srand(time(NULL));
	clear_screen();
	struct worldtile *world=worldgen(erosion,offset);
	draw_world(world);
	free(world);
	sgr(RESET);
	putchar('\n');
	return 0;
}
