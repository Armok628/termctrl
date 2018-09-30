#include "world.h"
short avg_elev(short *elevs,int pos)
{
	int sum=0;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++)
			sum+=elevs[pos+dx+dy*WIDTH];
	for (int dx=-1;dx<=1;dx++)
		sum+=elevs[pos+dx];
	for (int dy=-1;dy<=1;dy++)
		sum+=elevs[pos+dy*WIDTH];
	return sum/15;
}
void erode(struct worldtile *w)
{
	short elevs[AREA];
	for (int x=0;x<WIDTH;x++)
		for (int y=0;y<HEIGHT;y++) {
			int i=x+y*WIDTH;
			elevs[i]=w[i].elev;
		}
	for (int x=1;x<WIDTH-1;x++)
		for (int y=1;y<HEIGHT-1;y++) {
			int i=x+y*WIDTH;
			w[i].elev=avg_elev(elevs,i);
		}
}
struct worldtile *worldgen(void)
{
	struct worldtile *w=calloc(AREA,sizeof(struct worldtile));
	for (int x=1;x<WIDTH-1;x++)
		for (int y=1;y<HEIGHT-1;y++) {
			int i=x+y*WIDTH;
			w[i].elev=rand()%1000;
			w[i].temp=rand()%1000;
		}
	for (int i=0;i<5;i++)
		erode(w);
	return w;
}
void draw_worldtile(struct worldtile t)
{
	short elev=t.elev;
	//short temp=t.temp;
	sgr(RESET);
	sgr(BG_BLACK);
	char sym=' ';
	if (elev<200) { // Void
		sgr(FG_BLACK);
	} else if (elev<500) { // Sea
		sym='_';
		sgr(FG_BLUE);
		if (elev>475)
			sgr(BOLD);
	} else if (elev<590) { // Lowlands
		sym='~';
		if (elev<520) // Sand
			sgr(FG_YELLOW);
		else // Grass
			sgr(FG_GREEN);
		if (elev<550) // Meadows/Beaches
			sgr(BOLD);
		else if (570<=elev&&elev<=580) // Forests
			sym='%';
	} else { // Highlands
		if (elev<610) { // Low Mountains
			sgr(FG_BLACK);
			sgr(BOLD);
			sym='-';
		} else if (elev<640) { // High Mountains
			sgr(FG_WHITE);
			sym='=';
		} else { // Summits
			sgr(FG_WHITE);
			sgr(BOLD);
			sym='^';
		}
	}
	putchar(sym);
}
void draw_world(struct worldtile *w)
{
	for (int i=0;i<AREA;i++) {
		move_cursor(i%WIDTH,i/WIDTH);
		draw_worldtile(w[i]);
	}
}
