#include "room.h"
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
		case ' ':
			printf("\033[0;40m");
			break;
		}
		putchar(area[i]);
		if (i%WIDTH==WIDTH-1)
			printf("\033\[m\n");
	}
	printf("\033[m");
}
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
void place_doors(char *area,int x,int y,int w,int h)
{
	int n=1+rand()%4;
	while (n-->0) {
		switch (rand()%4) {
		case 0:
			area[x+w/2+y*WIDTH]='+';
			break;
		case 1:
			area[x+w/2+(y+h-1)*WIDTH]='+';
			break;
		case 2:
			area[x+w-1+(y+h/2)*WIDTH]='+';
			break;
		case 3:
			area[x+(y+h/2)*WIDTH]='+';
			break;
		}
	}
}
#define MAX_PARTS 3
#define N_PARTS (2+rand()%(MAX_PARTS-2))
#define MIN_WIDTH 3
#define RECURSE_FACTOR 5
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
		else
			place_doors(area,l,y,pw,h);
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
		else
			place_doors(area,x,l,w,ph);
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
