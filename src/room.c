#include "room.h"
void make_room(char *area,int x,int y,int w,int h)
{
	for (int i=x;i<x+w;i++)
		for (int j=y;j<y+h;j++)
			area[i+j*WIDTH]='#';
	for (int i=x;i<x+w;i++) {
		area[i+y*WIDTH]='%';
		area[i+(y+h-1)*WIDTH]='%';
	}
	for (int j=y;j<y+h;j++) {
		area[x+j*WIDTH]='%';
		area[(x+w-1)+j*WIDTH]='%';
	}
}
#define MIN_BOUNDS 2
#define MAX_BOUNDS 3
#define N_BOUNDS() (MIN_BOUNDS+rand()%(MAX_BOUNDS-MIN_BOUNDS+1))
#define MIN_RECURSE_WIDTH 6
#define RECURSE_FACTOR 4
void partition_room(char *area,int x,int y,int w,int h)
{
	int bounds[MAX_BOUNDS];
	int n=N_BOUNDS();
	// TODO: Refactor
	if (w>h) {
		rand_fixed_sum(bounds,n,w-1);
		int l=x,r=l;
		for (int i=0;i<n+1;i++) {
			if (i<n) {
				r+=bounds[i];
				if (r-l<2||x+w-1-r<2)
					continue;
				for (int j=y;j<y+h;j++)
					area[r+j*WIDTH]='%';
			} else
				r=x+w-1;
			int pw=r-l;
			if (pw>MIN_RECURSE_WIDTH&&pw>w/RECURSE_FACTOR)
				partition_room(area,l,y,pw+1,h);
			l=r;
		}
	} else {
		rand_fixed_sum(bounds,n,h-1);
		int l=y,r=l;
		for (int i=0;i<n+1;i++) {
			if (i<n) {
				r+=bounds[i];
				if (r-l<2||y+h-1-r<2)
					continue;
				for (int j=x;j<x+w;j++)
					area[j+r*WIDTH]='%';
			} else
				r=y+h-1;
			int ph=r-l;
			if (ph>MIN_RECURSE_WIDTH&&ph>h/RECURSE_FACTOR)
				partition_room(area,x,l,w,ph+1);
			l=r;
		}
	}
}
void place_doors(char *area)
{
	int *reached=malloc(AREA*sizeof(int));
	int *doors=malloc(AREA*sizeof(int));
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
		int n_doors=0;
		for (int i=0;i<AREA;i++) {
			if (reached[i]!=-1)
				continue;
			if ((reached[i+WIDTH]>0&&reached[i-WIDTH]==0)
					||(reached[i+WIDTH]==0&&reached[i-WIDTH]>0)
					||(reached[i+1]>0&&reached[i-1]==0)
					||(reached[i+1]==0&&reached[i-1]>0))
				doors[n_doors++]=i;
			// ^ TODO: Refactor ugly condition
		}
		/* Print reachability matrix (TEMPORARY) */
		/*****************************************/
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
		printf("Placing %d doors\n",1+n_doors/48);
		clock_t t=clock();
		while (clock()-t<CLOCKS_PER_SEC/2);
		/*****************************************/
		// Pick one or more at random and place a door
		if (!n_doors)
			break;
		for (int i=0;i<1+n_doors/32;i++)
			area[doors[rand()%n_doors]]='+';
	}
	free(reached);
	free(doors);
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

