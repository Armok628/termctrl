#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/room.h"
#include "src/path.h"
char area[AREA];
void random_room(char *area)
{
	int x=1+rand()%(WIDTH-9);
	int y=1+rand()%(HEIGHT-9);
	int w=10+rand()%(WIDTH/4);
	int h=10+rand()%(HEIGHT/4);
	if (x+w>WIDTH-1)
		x-=w;
	if (y+h>HEIGHT-1)
		y-=h;
	make_room(area,x,y,w,h);
	partition_room(area,x,y,w,h);
}
int main(int argc,char **argv)
{
	srand(time(NULL));
	for (int i=0;i<AREA;i++)
		area[i]=' ';
	for (int i=0;i<8;i++)
		random_room(area);
	int from=rand()%AREA,to=rand()%AREA;
	area[from]='O';
	area[to]='X';
	print_area(area);
	printf("From %d to %d\n",from,to);
	clock_t before=clock();
	bool b=plan_path(area,from,to);
	printf("A path was%sfound\n",b?" ":" not ");
	printf("Pathfinding took %f ms\n",(double)(clock()-before)*1000.0/CLOCKS_PER_SEC);
	print_dists();
	printf("\033[m");
}
