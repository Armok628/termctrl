#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "src/world.h"
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
	draw_world(world);
	int h=0,n=0,c=0;
	for (int i=0;i<W_AREA;i++)
		if (world[i].elev>=500)
			switch (terrain_type(world[i])%3) {
			case 2:
				h++;
				break;
			case 1:
				n++;
				break;
			case 0:
				c++;
				break;
			}
	free(world);
	sgr(RESET);
	putchar('\n');
	printf("Hot land: %d\nNormal land: %d\nCold land: %d\n",h,n,c);
	printf("seed=%lu\n",seed);
	return 0;
}
