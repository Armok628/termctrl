#include "room.h"
void make_room(char *area,int x,int y,int w,int h)
{
	// Make floors
	for (int i=x;i<x+w;i++)
		for (int j=y;j<y+h;j++)
			area[i+j*WIDTH]='#';
	// North and South walls
	for (int i=x;i<x+w;i++) {
		area[i+y*WIDTH]='%';
		area[i+(y+h-1)*WIDTH]='%';
	}
	// West and East walls
	for (int j=y;j<y+h;j++) {
		area[x+j*WIDTH]='%';
		area[(x+w-1)+j*WIDTH]='%';
	}
}
#define RECURSE_LENGTH 8
#define RECURSE_FACTOR 4
void partition_room(char *area,int x,int y,int w,int h)
{
	if (w>h) {
		int b=x+2+rand()%(w-4); // Boundary x-coordinate
		// Create vertical wall
		for (int i=y+1;i<y+h-1;i++)
			area[b+i*WIDTH]='%';
		// If first half meets recursion criteria, recurse
		if (b-x+1>=RECURSE_LENGTH&&b-x+1>=w/RECURSE_FACTOR)
			partition_room(area,x,y,b-x+1,h);
		// If second half meets recursion criteria, recurse
		if (x+w-b>=RECURSE_LENGTH&&x+w-b>=w/RECURSE_FACTOR)
			partition_room(area,b,y,x+w-b,h);
	} else {
		int b=y+2+rand()%(h-4); // Boundary y-coordinate
		// Create horizontal wall
		for (int i=x+1;i<x+w-1;i++)
			area[i+b*WIDTH]='%';
		// If first half meets recursion criteria, recurse
		if (b-y+1>=RECURSE_LENGTH&&b-y+1>=h/RECURSE_FACTOR)
			partition_room(area,x,y,w,b-y+1);
		// If second half meets recursion criteria, recurse
		if (y+h-b>=RECURSE_LENGTH&&y+h-b>=h/RECURSE_FACTOR)
			partition_room(area,x,b,w,y+h-b);
	}
}
void place_doors(char *area)
{
	int *reached=malloc(AREA*sizeof(int));
	int *in_doors=malloc(AREA*sizeof(int));
	int *ex_doors=malloc(AREA*sizeof(int));
	bool unreachable=false;
	while (1) {
		// Setup reachability matrix
		for (int i=0;i<AREA;i++)
			reached[i]=area[i]=='%'?-1:0;
		reached[0]=1;
		// Detect unreachable areas
		bool done=false;
		for (int d=2;!done;d++) {
			done=true;
			unreachable=false;
			for (int i=0;i<AREA;i++) {
				if (!reached[i])
					unreachable=true;
				if (reached[i]!=d-1)
					continue;
				for (int dx=-1;dx<=1;dx++)
				for (int dy=-1;dy<=1;dy++) {
					int n=i+dx+dy*WIDTH;
					int xd=n%WIDTH-i%WIDTH;
					if (xd<-1||xd>1||n<0||n>=AREA)
						continue;
					if (!reached[n]) {
						reached[n]=d;
						done=false;
					}
				}
			}
		}
		// Find suitable door locations
		if (!unreachable)
			break;
		int n_ex_doors=0,n_in_doors=0;
		for (int i=0;i<AREA;i++) {
			if (reached[i]!=-1)
				continue;
			if ((reached[i+WIDTH]>0&&reached[i-WIDTH]==0)
					||(reached[i+WIDTH]==0&&reached[i-WIDTH]>0)
					||(reached[i+1]>0&&reached[i-1]==0)
					||(reached[i+1]==0&&reached[i-1]>0)) {
				if (area[i+WIDTH]=='+'||area[i-WIDTH]=='+'||
						area[i-1]=='+'||area[i+1]=='+')
					continue;
				if (area[i+WIDTH]==' '||area[i-WIDTH]==' '||
						area[i-1]==' '||area[i+1]==' ')
					ex_doors[n_ex_doors++]=i;
				else
					in_doors[n_in_doors++]=i;
			}
			// ^ TODO: Refactor those disgusting conditions
		}
		/* Visualize reachability matrix (TEMPORARY) */
		/*********************************************/
		clear_screen();
		move_cursor(0,0);
		for (int i=0;i<AREA;i++) {
			switch (reached[i]) {
			case -1:
				printf("\033[1;30;40m");
				putchar('#');
				break;
			case 0:
				printf("\033[0;31;40m");
				putchar('X');
				break;
			default:
				printf("\033[0;40m");
				putchar(' ');
			}
			if (i%WIDTH==WIDTH-1)
				putchar('\n');
		}
		printf("Placing %d %s doors\n",
				n_in_doors?1+n_in_doors/32:1+n_ex_doors/64,
				n_in_doors?"inside":"outside");
		clock_t t=clock();
		while (clock()-t<CLOCKS_PER_SEC/2);
		/*********************************************/
		// Pick one or more at random and place a door
		if (!n_in_doors&&n_ex_doors) {
			for (int i=0;i<1+n_ex_doors/64;i++)
				area[ex_doors[rand()%n_ex_doors]]='+';
		} else if (n_in_doors) {
			for (int i=0;i<1+n_in_doors/32;i++)
				area[in_doors[rand()%n_in_doors]]='+';
		} else
			break;
	}
	free(reached);
	free(in_doors);
	free(ex_doors);
}
void random_room(char *area)
{
	int w=10+rand()%(WIDTH/4);
	int h=10+rand()%(HEIGHT/4);
	int x=1+rand()%(WIDTH-w-1);
	int y=1+rand()%(HEIGHT-h-1);
	make_room(area,x,y,w,h);
	partition_room(area,x,y,w,h);
}
//// Temporary
void print_area(char *area)
{
	for (int i=0;i<AREA;i++) {
		switch (area[i]) {
		case '#':
			printf("\033[0;37;40m");
			break;
		case '%':
			printf("\033[1;30;40m");
			break;
		case '+':
			printf("\033[0;33;40m");
			break;
		case 'X':
			printf("\033[0;31;40m");
			break;
		case 'O':
			printf("\033[0;34;40m");
			break;
		case '*':
			printf("\033[1;33;40m");
			break;
		case ' ':
			printf("\033[0;40m");
			break;
		}
		putchar(area[i]);
		if (i%WIDTH==WIDTH-1)
			printf("\033[m\n");
	}
	printf("\033[m");
}

