#include "room.h"
void make_wall(struct tile *t)
{
	t->fg='%';
	t->fg_c=DARK_GRAY;
	t->bg='%';
	t->bg_c=DARK_GRAY;
}
void make_floor(struct tile *t)
{
	t->fg='#';
	t->fg_c=LIGHT_GRAY;
	t->bg='#';
	t->bg_c=LIGHT_GRAY;
}
void make_door(struct tile *t)
{
	t->fg='+';
	t->fg_c=BROWN;
	t->bg='-';
	t->bg_c=BROWN;
}
void make_room(struct tile *area,int x,int y,int w,int h)
{
	// Make floors
	for (int i=x;i<x+w;i++)
		for (int j=y;j<y+h;j++)
			make_floor(&area[i+j*ZONE_WIDTH]);
	// North and South walls
	for (int i=x;i<x+w;i++) {
		make_wall(&area[i+y*ZONE_WIDTH]);
		make_wall(&area[i+(y+h-1)*ZONE_WIDTH]);
	}
	// West and East walls
	for (int j=y;j<y+h;j++) {
		make_wall(&area[x+j*ZONE_WIDTH]);
		make_wall(&area[(x+w-1)+j*ZONE_WIDTH]);
	}
}
#define RECURSE_LENGTH 8
#define RECURSE_FACTOR 4
void partition_room(struct tile *area,int x,int y,int w,int h)
{
	if (w>h) {
		int b=x+2+rand()%(w-4); // Boundary x-coordinate
		// Create vertical wall
		for (int i=y+1;i<y+h-1;i++)
			make_wall(&area[b+i*ZONE_WIDTH]);
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
			make_wall(&area[i+b*ZONE_WIDTH]);
		// If first half meets recursion criteria, recurse
		if (b-y+1>=RECURSE_LENGTH&&b-y+1>=h/RECURSE_FACTOR)
			partition_room(area,x,y,w,b-y+1);
		// If second half meets recursion criteria, recurse
		if (y+h-b>=RECURSE_LENGTH&&y+h-b>=h/RECURSE_FACTOR)
			partition_room(area,x,b,w,y+h-b);
	}
}
bool is_wall(struct tile *t)
{
	return t->fg=='%';
}
bool is_door(struct tile *t)
{
	return t->fg=='+';
}
void place_doors(struct tile *area)
{
	int *reached=malloc(ZONE_AREA*sizeof(int));
	int *in_doors=malloc(ZONE_AREA*sizeof(int));
	int *ex_doors=malloc(ZONE_AREA*sizeof(int));
	bool unreachable=false;
	while (1) {
		// Setup reachability matrix
		for (int i=0;i<ZONE_AREA;i++)
			reached[i]=area[i].fg=='%'?-1:0;
		reached[0]=1;
		// Detect unreachable areas
		bool done=false;
		for (int d=2;!done;d++) {
			done=true;
			unreachable=false;
			for (int i=0;i<ZONE_AREA;i++) {
				if (!reached[i])
					unreachable=true;
				if (reached[i]!=d-1)
					continue;
				for (int dx=-1;dx<=1;dx++)
				for (int dy=-1;dy<=1;dy++) {
					int n=i+dx+dy*ZONE_WIDTH;
					int xd=n%ZONE_WIDTH-i%ZONE_WIDTH;
					if (xd<-1||xd>1||n<0||n>=ZONE_AREA)
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
		for (int i=0;i<ZONE_AREA;i++) {
			if (reached[i]!=-1)
				continue;
			if ((reached[i+ZONE_WIDTH]>0&&reached[i-ZONE_WIDTH]==0)
					||(reached[i+ZONE_WIDTH]==0&&reached[i-ZONE_WIDTH]>0)
					||(reached[i+1]>0&&reached[i-1]==0)
					||(reached[i+1]==0&&reached[i-1]>0)) {
				if (!area[i+ZONE_WIDTH].fg||!area[i-ZONE_WIDTH].fg||
						!area[i-1].fg||!area[i+1].fg)
					ex_doors[n_ex_doors++]=i;
				else
					in_doors[n_in_doors++]=i;
			}
			// ^ TODO: Refactor those disgusting conditions
		}
		// Pick one or more at random and place a door
		if (!n_in_doors&&n_ex_doors) {
			for (int i=0;i<1+n_ex_doors/64;i++)
				make_door(&area[ex_doors[rand()%n_ex_doors]]);
		} else if (n_in_doors) {
			for (int i=0;i<1+n_in_doors/32;i++)
				make_door(&area[in_doors[rand()%n_in_doors]]);
		} else
			break;
	}
	free(reached);
	free(in_doors);
	free(ex_doors);
}
void random_room(struct tile *area)
{
	int w=10+rand()%(ZONE_WIDTH/4);
	int h=10+rand()%(ZONE_HEIGHT/4);
	int x=1+rand()%(ZONE_WIDTH-w-1);
	int y=1+rand()%(ZONE_HEIGHT-h-1);
	make_room(area,x,y,w,h);
	partition_room(area,x,y,w,h);
}
