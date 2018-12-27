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
	int from=rand()%AREA,to=rand()%AREA;
	area[from]='O';
	area[to]='X';
	printf("From %d to %d\n",from,to);
	clock_t before=clock();
	bool b=plan_path(area,from,to);
	printf("A path was%sfound\n",b?" ":" not ");
	printf("Pathfinding took %f ms\n",(double)(clock()-before)*1000.0/CLOCKS_PER_SEC);
	if (b) {
		show_path(area,from,to);
		print_area(area);
	} else {
		print_area(area);
		print_dists();
	}
}
