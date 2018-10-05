#include "world.h"
bool show_climates=false;
short avg_around(short *elevs,int pos)
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
			w[i].elev=avg_around(elevs,i);
			w[i].temp=avg_around(temps,i);
		}
}
struct worldtile *worldgen(int age,int e_o,int t_o,float e_f)
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
	if (e_f!=1.0)
		for (int i=0;i<W_AREA;i++)
			w[i].elev=500+(w[i].elev-500)*e_f;
	return w;
}
enum terrain terrain_type(struct worldtile tile)
{
	short elev=tile.elev;
	short temp=tile.temp;
	enum terrain t=VOID;
	sgr(RESET);
	sgr(BG_BLACK);
	if (elev<200) { // Void
		t=VOID;
	} else if (elev<500) { // Sea
		t=elev>475?SHALLOW_SEA:DEEP_SEA;
	} else if (elev<590) { // Lowlands
		if (elev<520)
			t=BEACH;
		else if (elev<550)
			t=MEADOW;
		else
			t=FIELD;
		if (570<=elev&&elev<=580) // Forests
			t=FOREST;
	} else { // Highlands
		if (elev<610)
			t=LOW_MOUNTAIN;
		else if (elev<640)
			t=HIGH_MOUNTAIN;
		else
			t=SUMMIT;
	}
	if (temp<400) // Cold
		t--;
	else if (temp>625) // Hot
		t++;
	return t;
}
void draw_terrain(enum terrain t)
{
	sgr(RESET);
	sgr(BG_BLACK);
	if (show_climates) {
		switch (t%3) {
		case 2:
			sgr(BG_RED);
			break;
		case 0:
			sgr(BG_BLUE);
		}
	}
	switch (t) {
	// Nothing
	case COLD_VOID:
	case VOID:
	case HOT_VOID:
		putchar(' ');
		break;
	// Sea
	case COLD_SHALLOW_SEA:
		sgr(BOLD);
	case COLD_DEEP_SEA:
		sgr(FG_CYAN);
		putchar('_');
		break;
	case SHALLOW_SEA:
	case HOT_SHALLOW_SEA:
		sgr(BOLD);
	case DEEP_SEA:
	case HOT_DEEP_SEA:
		sgr(FG_BLUE);
		putchar('_');
		break;
	// Sand
	case COLD_BEACH:
		sgr(BOLD);
		sgr(FG_GRAY);
		putchar('~');
		break;
	case BEACH:
	case HOT_BEACH:
		sgr(BOLD);
		sgr(FG_YELLOW);
		putchar('~');
		break;
	// Meadows / Fields
	case COLD_MEADOW:
		sgr(BOLD);
	case COLD_FIELD:
		sgr(FG_GRAY);
		putchar('~');
		break;
	case MEADOW:
		sgr(BOLD);
	case FIELD:
		sgr(FG_GREEN);
		putchar('~');
		break;
	case HOT_MEADOW:
		sgr(BOLD);
	case HOT_FIELD:
		sgr(FG_YELLOW);
		putchar('~');
		break;
	// Forest
	case COLD_FOREST:
		sgr(BOLD);
		sgr(FG_GRAY);
		putchar('%');
		break;
	case FOREST:
		sgr(FG_GREEN);
		putchar('%');
		break;
	case HOT_FOREST:
		sgr(FG_YELLOW);
		putchar('%');
		break;
	// Low Mountain
	case COLD_LOW_MOUNTAIN:
		sgr(BOLD);
		sgr(FG_GRAY);
		putchar('-');
		break;
	case LOW_MOUNTAIN:
		sgr(BOLD);
		sgr(FG_BLACK);
		putchar('-');
		break;
	case HOT_LOW_MOUNTAIN:
		sgr(FG_GRAY);
		putchar('-');
		break;
	// High Mountain
	case COLD_HIGH_MOUNTAIN:
	case HIGH_MOUNTAIN:
		sgr(BOLD);
	case HOT_HIGH_MOUNTAIN:
		sgr(FG_GRAY);
		putchar('=');
		break;
	// Summit
	case COLD_SUMMIT:
	case SUMMIT:
		sgr(BOLD);
	case HOT_SUMMIT:
		sgr(FG_GRAY);
		putchar('^');
		break;
	}
}
void draw_world(struct worldtile *w)
{
	for (int i=0;i<W_AREA;i++) {
		move_cursor(i%W_WIDTH,i/W_WIDTH);
		draw_terrain(terrain_type(w[i]));
	}
}
