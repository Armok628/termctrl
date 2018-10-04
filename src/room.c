#include "room.h"
void make_floor(struct tile *t)
{
	t->fg_sym='\0';
	t->bg_sym='#';
	t->bg_gr[0]=FG_GRAY;
	t->bg_gr[1]=RESET;
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
void make_door(struct tile *t)
{
	t->fg_sym='+';
	t->fg_gr[0]=FG_YELLOW;
	t->fg_gr[1]=RESET;
	t->bg_sym='-';
	t->bg_gr[0]=FG_YELLOW;
	t->bg_gr[1]=RESET;
}
void room(struct tile *z,int x,int y,int w,int h)
{
	for (int i=x;i<x+w;i++)
		for (int j=y;j<y+h;j++) {
			make_floor(&z[i+j*WIDTH]);
		}
	for (int i=x;i<x+w;i++) {
		make_wall(&z[i+y*WIDTH]);
		make_wall(&z[i+(y+h-1)*WIDTH]);
	}
	for (int j=y;j<y+h;j++) {
		make_wall(&z[x+j*WIDTH]);
		make_wall(&z[x+w-1+j*WIDTH]);
	}
	switch (rand()%4) {
	case 0: // North
		make_door(&z[x+w/2+y*WIDTH]);
		break;
	case 1: // South
		make_door(&z[x+w/2+(y+h-1)*WIDTH]);
		break;
	case 2: // East
		make_door(&z[x+w-1+(y+h/2)*WIDTH]);
		break;
	case 3: // West
		make_door(&z[x+(y+h/2)*WIDTH]);
		break;
	}
}
