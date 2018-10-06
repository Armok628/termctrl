#include "room.h"
void make_floor(struct tile *t)
{
	t->fg='\0';
	t->bg='#';
	t->bg_c=LIGHT_GRAY;
}
void make_wall(struct tile *t)
{
	t->fg='%';
	t->fg_c=DARK_GRAY;
	t->bg='%';
	t->bg_c=DARK_GRAY;
}
void make_door(struct tile *t)
{
	t->fg='+';
	t->fg_c=BROWN;
	t->bg='-';
	t->bg_c=BROWN;
}
void room(struct tile *z,int x,int y,int w,int h,int dir)
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
	switch (dir) {
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
void rand_room(struct tile *z)
{
	int x=1+rand()%(WIDTH-2);
	int y=1+rand()%(HEIGHT-2);
	int w=3+rand()%(WIDTH/4);
	int h=3+rand()%(HEIGHT/4);
	if (x+w>WIDTH-1)
		x-=w;
	if (y+h>HEIGHT-1)
		y-=h;
	room(z,x,y,w,h,rand()%4);
}
