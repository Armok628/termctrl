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
#define MIN_RECURSE_WIDTH 6
#define RECURSE_FACTOR 4
void partition_room(char *area,int x,int y,int w,int h)
{
	int bounds[2];
	// TODO: Refactor (heavily)
	if (w>h) {
		rand_fixed_sum(bounds,2,w-1);
		int l=x,r=l;
		for (int i=0;i<3;i++) {
			if (i<2) {
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
		rand_fixed_sum(bounds,2,h-1);
		int l=y,r=l;
		for (int i=0;i<3;i++) {
			if (i<2) {
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
	int *in_doors=malloc(AREA*sizeof(int));
	int *out_doors=malloc(AREA*sizeof(int));
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
		int n_out_doors=0,n_in_doors=0;
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
					out_doors[n_out_doors++]=i;
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
				n_in_doors?1+n_in_doors/32:1+n_out_doors/64,
				n_in_doors?"inside":"outside");
		clock_t t=clock();
		while (clock()-t<CLOCKS_PER_SEC/2);
		/*********************************************/
		// Pick one or more at random and place a door
		if (!n_in_doors&&n_out_doors) {
			for (int i=0;i<1+n_out_doors/64;i++)
				area[out_doors[rand()%n_out_doors]]='+';
		} else if (n_in_doors) {
			for (int i=0;i<1+n_in_doors/32;i++)
				area[in_doors[rand()%n_in_doors]]='+';
		} else
			break;
	}
	free(reached);
	free(in_doors);
	free(out_doors);
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

