#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/world.h"
#include "src/terminal.h"
int main(/*int argc,char **argv*/)
{
	srand(time(NULL));
	clear_screen();
	struct worldtile *world=worldgen();
	draw_world(world);
	free(world);
	sgr(RESET);
	putchar('\n');
	return 0;
}
