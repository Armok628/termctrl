#include "room.h"
static inline int lin(int x,int y)
{
	return x+y*WIDTH;
}
void make_door(struct tile *t)
{
	t->fg_sym='+';
	t->fg_gr[0]=FG_YELLOW;
	t->fg_gr[1]=NO_BOLD;
	t->bg_sym='-';
	t->bg_gr[0]=FG_YELLOW;
	t->bg_gr[1]=NO_BOLD;
}
void make_floor(struct tile *t)
{
	t->fg_sym='\0';
	t->bg_sym='#';
	t->bg_gr[0]=FG_GRAY;
	t->bg_gr[1]=NO_BOLD;
}
void make_wall(struct tile *t)
{
	t->fg_sym='%';
	t->fg_gr[0]=FG_BLACK;
	t->fg_gr[1]=BOLD;
	t->bg_sym='%';
	t->bg_gr[0]=FG_BLACK;
	t->bg_gr[1]=BOLD;
}
void make_room(struct tile *a,int x,int y,int w,int h,enum dir direction)
{
	for (int xo=0;xo<w;xo++)
		for (int yo=0;yo<h;yo++)
			make_floor(&a[lin(x+xo,y+yo)]);
	for (int i=0;i<w;i++) {
		make_wall(&a[lin(x+i,y)]);
		make_wall(&a[lin(x+i,y+h-1)]);
	}
	for (int i=0;i<h;i++) {
		int l=lin(x,y+i);
		make_wall(&a[l]);
		l=lin(x+w-1,y+i);
		make_wall(&a[l]);
	}
	switch (direction) {
	case NORTH:
		make_door(&a[lin(x+w/2,y)]);
		break;
	case SOUTH:
		make_door(&a[lin(x+w/2,y+h-1)]);
		break;
	case EAST:
		make_door(&a[lin(x+w-1,y+h/2)]);
		break;
	case WEST:
		make_door(&a[lin(x,y+h/2)]);
	}
}
void random_room(struct tile *z)
{
	int w=3+rand()%(WIDTH/4);
	int h=3+rand()%(HEIGHT/4);
	int x=1+rand()%(WIDTH-w-2);
	int y=1+rand()%(HEIGHT-h-2);
	if (x+w>WIDTH)
		x-=w;
	if (y+h>HEIGHT)
		y-=h;
	make_room(z,x,y,w,h,(enum dir)rand()%4);
}
bool wall_needs_cull(struct tile *z,int i)
{
	if (z[i].fg_sym!='%'&&z[i].fg_sym!='+')
		return false;
	int hw=0,vw=0,walls=0,floors=0,doors=0,empty_space=0;
	for (int x=-1;x<=1;x++)
		for (int y=-1;y<=1;y++) {
			int c=i+lin(x,y);
			vw+=!x&&y&&z[c].fg_sym=='%';
			hw+=!y&&x&&z[c].fg_sym=='%';
			walls+=z[c].fg_sym=='%';
			floors+=z[c].bg_sym=='#';
			doors+=z[c].fg_sym=='+';
			empty_space+=z[c].fg_sym==' ';
		}
	// Inside room
	switch (z[i].fg_sym) {
	case '+':
		if (empty_space)
			return true;
		if ((hw==2&&!vw)||(!hw&&vw==2))
			return false;
		if (hw==1||vw==1)
			return true;
		if (doors>1||walls<2||walls>4)
			return true;
		return false;
	case '%':
		if (walls+floors+doors<9) // Border of room
			return false;
		if (walls>3)//&&!doors)
			return true;
		if (walls<2)
			return true;
		return false;
	default:
		return false;
	}
}
void fix_gap(struct tile *z,int i)
{
	if (z[i].bg_sym!='#')
		return;
	int hw=0,vw=0,walls=0,floors=0,doors=0,empty_space=0;
	for (int x=-1;x<=1;x++)
		for (int y=-1;y<=1;y++) {
			int c=i+lin(x,y);
			vw+=z[c].fg_sym=='%'&&!x&&y;
			hw+=z[c].fg_sym=='%'&&!y&&x;
			walls+=z[c].fg_sym=='%';
			walls+=z[c].fg_sym==' ';
			empty_space+=z[c].fg_sym==' ';
			floors+=z[c].bg_sym=='#';
			doors+=z[c].fg_sym=='+';
		}
	if (empty_space)
		make_wall(&z[i]);
	if (walls+floors+doors<9) { // Border of room
		if (!doors)
			make_door(&z[i]);
		else
			make_wall(&z[i]);
	} else { // Inside room
		if (!doors&&walls<5&&((hw==2&&!vw)||(!hw&&vw==2)))
			make_door(&z[i]);
		else if (hw==1&&vw==1)
			make_wall(&z[i]);
	}
}
int cull_walls(struct tile *z)
{
	int walls_removed=0;
	for (int i=WIDTH+1;i<AREA-WIDTH-1;i++)
		if (wall_needs_cull(z,i)) {
			z[i].fg_sym='\0';
			z[i].bg_sym='#';
			z[i].bg_gr[0]=FG_GRAY;
			z[i].bg_gr[1]=NO_BOLD;
			walls_removed++;
		}
	return walls_removed;
}
void fix_rooms(struct tile *z)
{
	while (cull_walls(z));
	for (int i=0;i<4;i++)
		for (int c=0;c<AREA;c++)
			fix_gap(z,c);
	while (cull_walls(z));
}
int dist_to_room(struct tile *z,int c,char dir)
{
	int o=input_offset(dir),d=1;
	while (legal_move(c,c+o)) {
		c+=o;
		char bg=z[c].bg_sym;
		if (bg=='#'||bg=='%'||z[c].fg_sym=='+')
			return d;
		d++;
	}
	return -1;
}
void floor_line(struct tile *z,int c,int l,char dir)
{
	if (l<0)
		return;
	make_floor(&z[c]);
	int o=input_offset(dir);
	for (int i=0;i<l;i++)
		make_floor(&z[c+=o]);
	make_door(&z[c]);
}
bool make_path(struct tile *z,int c)
{
	if (z[c].bg_sym=='#'||z[c].bg_sym=='%')
		return false;
	int h=dist_to_room(z,c,'4');
	int j=dist_to_room(z,c,'2');
	int k=dist_to_room(z,c,'8');
	int l=dist_to_room(z,c,'6');
	if ((h>0)+(j>0)+(k>0)+(l>0)<2)
		return false; // Not enough possible directions
	// Make actual paths
	if ((h>=j)+(h>=k)+(h>=l)>=2)
		floor_line(z,c,h,'h');
	if ((j>=h)+(j>=k)+(j>=l)>=2)
		floor_line(z,c,j,'j');
	if ((k>=h)+(k>=j)+(k>=l)>=2)
		floor_line(z,c,k,'k');
	if ((l>=h)+(l>=j)+(l>=k)>=2)
		floor_line(z,c,l,'l');
	return true;
}
void random_path(struct tile *z)
{ // Keep trying random coordinates until success
	while (!make_path(z,rand()%AREA));
}
