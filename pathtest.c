#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/room.h"
#include "src/path.h"
char area[AREA];
int main(int argc,char **argv)
{
	unsigned int seed;
	if (argc>1)
		sscanf(argv[1],"seed=%u",&seed);
	else
		seed=time(NULL);
	srand(seed);
	printf("Seed: %u\n",seed);
	for (int i=0;i<AREA;i++)
		area[i]=' ';
	for (int i=0;i<8;i++)
		random_room(area);
	place_doors(area);
	int from=rand()%AREA,to=rand()%AREA;
	/* Warning: O(inf) ahead */
	while (area[from]!='#') from=rand()%AREA;
	while (area[to]!='#') to=rand()%AREA;
	/**/
	area[from]='O';
	area[to]='X';
	clock_t t=clock();
	show_path(area,from,to);
	t=clock()-t;
	printf("Pathfinding took %f ms\n",1000.0*t/CLOCKS_PER_SEC);
	print_dists();
	putchar('\n');
	print_area(area);
}
