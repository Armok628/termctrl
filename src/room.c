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
#define MAX_PARTS 3
#define N_PARTS (2+rand()%(MAX_PARTS-2))
#define MIN_WIDTH 3
#define RECURSE_FACTOR 5
void partition_room_horiz(char *,int,int,int,int);
void partition_room_vert(char *area,int x,int y,int w,int h)
{ // Vertical room boundaries
	int bounds[MAX_PARTS];
	int n=N_PARTS;
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
		int pw=r-l+1;
		if (pw>MIN_WIDTH&&pw>w/RECURSE_FACTOR&&rand()%3)
			partition_room_horiz(area,l,y,pw,h);
		l=r;
	}
}
void partition_room_horiz(char *area,int x,int y,int w,int h)
{ // Horizontal room boundaries
	int bounds[MAX_PARTS];
	int n=N_PARTS;
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
		int ph=r-l+1;
		if (ph>MIN_WIDTH&&ph>h/RECURSE_FACTOR&&rand()%3)
			partition_room_vert(area,x,l,w,ph);
		l=r;
	}
}
void partition_room(char *area,int x,int y,int w,int h)
{
	if (rand()&1)
		partition_room_vert(area,x,y,w,h);
	else
		partition_room_horiz(area,x,y,w,h);
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
		if (!unreachable)
			break;
		// Find suitable door locations
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
		// Pick one at random and place a door
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

