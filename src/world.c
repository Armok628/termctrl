#include "world.h"
bool show_climates=false;
short avg_elev(short *elevs,int pos)
{
	int sum=0;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++)
			sum+=elevs[pos+dx+dy*W_WIDTH];
	for (int dx=-1;dx<=1;dx++)
		sum+=elevs[pos+dx];
	for (int dy=-1;dy<=1;dy++)
		sum+=elevs[pos+dy*W_WIDTH];
	return sum/15;
}
void erode(struct worldtile *w)
{
	short elevs[W_AREA];
	short temps[W_AREA];
	for (int x=0;x<W_WIDTH;x++)
		for (int y=0;y<W_HEIGHT;y++) {
			int i=x+y*W_WIDTH;
			elevs[i]=w[i].elev;
			temps[i]=w[i].temp;
		}
	for (int x=1;x<W_WIDTH-1;x++)
		for (int y=1;y<W_HEIGHT-1;y++) {
			int i=x+y*W_WIDTH;
			w[i].elev=avg_elev(elevs,i);
			w[i].temp=avg_elev(temps,i);
		}
}
struct worldtile *worldgen(int age,int e_o,int t_o)
{
	struct worldtile *w=calloc(W_AREA,sizeof(struct worldtile));
	for (int y=W_HEIGHT/5+2;y<W_HEIGHT*4/5;y++) {
		w[y*W_WIDTH].temp=500;
		w[(y+1)*W_WIDTH-1].temp=500;
	}
	for (int x=1;x<W_WIDTH-1;x++)
		for (int y=1;y<W_HEIGHT-1;y++) {
			// Generate values
			int e=e_o+rand()%1000;
			int t=y-W_HEIGHT/2;
			t=t<0?t:-t;
			t*=1000/W_HEIGHT;
			t+=300+rand()%800;
			t+=t_o;
			// Insert values
			int i=x+y*W_WIDTH;
			w[i].elev=e;
			w[i].temp=t;
		}
	for (int i=0;i<age;i++)
		erode(w);
	return w;
}
void draw_worldtile(struct worldtile t)
{ // TODO: Replace with climate enum and function to determine value
	short elev=t.elev;
	short temp=t.temp;
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
			sgr(FG_DEFAULT);
			sym='=';
		} else { // Summits
			if (elev>700)
				sgr(FG_RED);
			else
				sgr(FG_DEFAULT);
			sgr(BOLD);
			sym='^';
		}
	}
	if (temp<400) { // Cold
		if (show_climates)
			sgr(BG_BLUE);
		if (elev<500) { // Sea
			sgr(FG_CYAN);
			sgr(BOLD);
		} else // Land
			sgr(FG_WHITE);
	} else if (temp>625) { // Hot
		if (show_climates)
			sgr(BG_RED);
		if (elev>=590) { // Above Lowlands
			sgr(FG_GRAY);
			sgr(NO_BOLD);
		} else if (elev>=500) // Above Sea
			sgr(FG_YELLOW);
	}
	putchar(sym);
}
void draw_world(struct worldtile *w)
{
	for (int i=0;i<W_AREA;i++) {
		move_cursor(i%W_WIDTH,i/W_WIDTH);
		draw_worldtile(w[i]);
	}
}
