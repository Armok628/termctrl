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
			make_floor(&z[i+j*Z_WIDTH]);
		}
	for (int i=x;i<x+w;i++) {
		make_wall(&z[i+y*Z_WIDTH]);
		make_wall(&z[i+(y+h-1)*Z_WIDTH]);
	}
	for (int j=y;j<y+h;j++) {
		make_wall(&z[x+j*Z_WIDTH]);
		make_wall(&z[x+w-1+j*Z_WIDTH]);
	}
	switch (dir) {
	case 0: // North
		make_door(&z[x+w/2+y*Z_WIDTH]);
		break;
	case 1: // South
		make_door(&z[x+w/2+(y+h-1)*Z_WIDTH]);
		break;
	case 2: // East
		make_door(&z[x+w-1+(y+h/2)*Z_WIDTH]);
		break;
	case 3: // West
		make_door(&z[x+(y+h/2)*Z_WIDTH]);
		break;
	}
}
void rand_room(struct tile *z)
{
	int x=1+rand()%(Z_WIDTH-2);
	int y=1+rand()%(Z_HEIGHT-2);
	int w=3+rand()%(Z_WIDTH/4);
	int h=3+rand()%(Z_HEIGHT/4);
	if (x+w>Z_WIDTH-1)
		x-=w;
	if (y+h>Z_HEIGHT-1)
		y-=h;
	room(z,x,y,w,h,rand()%4);
}
bool needs_removal(struct tile *z,int pos)
{
	if (z[pos].fg!='%'&&z[pos].fg!='+')
		return false;
	int vw=0,hw=0,w=0,f=0,d=0,e=0;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++) {
			if (!dx&&!dy)
				continue;
			int i=pos+dx+dy*Z_WIDTH;
			vw+=!dx&&z[i].fg=='%';
			hw+=!dy&&z[i].fg=='%';
			w+=z[i].fg=='%';
			d+=z[i].fg=='+';
			e+=z[i].fg==' ';
			f+=z[i].bg=='#';
		}
	if (z[pos].fg=='%') { // Wall
		if (f+w+d<8)
			return false;
		if (w<1||3<w||hw+vw==1)
			return true;
		if (d&&w>1)
			return true;
	} else { // Door
		if (e||d>1)
			return true;
		if ((hw<2&&vw<2)||(vw&&hw))
			return true;
		if (w<2||4<w)
			return true;
	}
	return false;
}
void fix_rooms(struct tile *z)
{
	for (int i=0;i<Z_AREA;i++)
		if (needs_removal(z,i))
			make_floor(&z[i]);
}
void fix_gap(struct tile *z,int pos)
{
	int vw=0,hw=0,w=0,f=0,d=0,e=0;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++) {
			if (!dx&&!dy)
				continue;
			int i=pos+dx+dy*Z_WIDTH;
			vw+=!dx&&z[i].fg=='%';
			hw+=!dy&&z[i].fg=='%';
			w+=z[i].fg=='%';
			d+=z[i].fg=='+';
			e+=z[i].fg==' ';
			f+=z[i].bg=='#';
		}
	if (f+w+d<8)
		if (!d)
			make_door(&z[pos]);
		else
			make_wall(&z[pos]);
	else if (w<4&&vw==1&&hw==1)
		make_wall(&z[pos]);
	else if (w<2&&(vw||hw))
		make_wall(&z[pos]);
	else {
		make_door(&z[pos]);
		if (needs_removal(z,pos))
			make_floor(&z[pos]);
	}
}
void fix_gaps(struct tile *z)
{
	for (int i=0;i<Z_AREA;i++) {
		if (z[i].bg!='#')
			continue;
		fix_gap(z,i);
	}
}
int wall_dist(struct tile *z,int pos,char dir)
{
	int o=input_offset(dir);
	for (int i=0;legal_move(pos,pos+o);pos+=o,i++)
		if (z[pos].fg=='%')
			return i;
	return 0;
}
void path_to(struct tile *z,int pos,char dir)
{
	int o=input_offset(dir);
	for (;z[pos].fg!='%';pos+=o)
		make_floor(&z[pos]);
	make_door(&z[pos]);
}
bool path(struct tile *z,int pos)
{
	if (z[pos].bg=='#'||z[pos].bg=='%')
		return false;
	int h=wall_dist(z,pos,'h');
	int j=wall_dist(z,pos,'j');
	int k=wall_dist(z,pos,'k');
	int l=wall_dist(z,pos,'l');
	if (!h+!j+!k+!l>2)
		return false;
	if ((h>=j)+(h>=k)+(h>=l)>=2)
		path_to(z,pos,'h');
	if ((j>=h)+(j>=k)+(j>=l)>=2)
		path_to(z,pos,'j');
	if ((k>=h)+(k>=j)+(k>=l)>=2)
		path_to(z,pos,'k');
	if ((l>=h)+(l>=j)+(l>=k)>=2)
		path_to(z,pos,'l');
	return true;
}
void rand_path(struct tile *z)
{
	while (!path(z,rand()%Z_AREA));
}
