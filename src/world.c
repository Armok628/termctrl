#include "world.h"
short avg_around(short *elevs,int pos)
{
	int sum=0;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++)
			sum+=elevs[pos+dx+dy*W_WIDTH];
	return sum/9;
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
char descend(struct worldtile *w,int pos)
{ // Returns '1'-'9'
	int i=0,m=0,min=w[pos].elev;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++) {
			int p=pos+dx+dy*W_WIDTH;
			if (w[p].elev<min) {
				min=w[p].elev;
				m=i;
			}
			i++;
		}
	return m+'1';
} // To convert to enum dir, subtract '0'
void run_river(struct worldtile *w,int pos)
{
	while (w[pos].elev>500&&!w[pos].river) {
		char d=descend(w,pos);
		w[pos].river=d-'0';
		pos+=input_offset_width(d,W_WIDTH);
	}
}
struct worldtile *worldgen(int age,int e_o,int t_o,float e_f,float t_f)
{
	struct worldtile *w=calloc(W_AREA,sizeof(struct worldtile));
	for (int y=W_HEIGHT/5+2;y<W_HEIGHT*4/5;y++) {
		w[y*W_WIDTH].temp=500;
		w[(y+1)*W_WIDTH-1].temp=500;
	}
#ifdef HEMISPHERE
	for (int y=W_HEIGHT*4/5;y<W_HEIGHT;y++) {
		w[y*W_WIDTH].temp=1000;
		w[(y+1)*W_WIDTH-1].temp=1000;
	}
	for (int x=0;x<W_WIDTH;x++)
		w[x+(W_HEIGHT-1)*W_WIDTH].temp=1000;
#endif
	puts("Generating noisemaps");
	for (int x=1;x<W_WIDTH-1;x++)
		for (int y=1;y<W_HEIGHT-1;y++) {
			// Generate values
			int e=rand()%1000;
#ifdef HEMISPHERE
			int t=y-W_HEIGHT;
			t=t<0?-t:t;
			t*=-500/W_HEIGHT;
			t+=350+rand()%800;
#else
			int t=y-W_HEIGHT/2;
			t=t<0?-t:t;
			t*=-1000/W_HEIGHT;
			t+=300+rand()%800;
#endif
			// Insert values
			int i=x+y*W_WIDTH;
			w[i].elev=e;
			w[i].temp=t;
		}
	puts("Simulating erosion");
	for (int i=0;i<age;i++)
		erode(w);
	if (e_f!=1.0) {
		puts("Applying elevation factor");
		for (int i=0;i<W_AREA;i++)
			w[i].elev=500+(w[i].elev-500)*e_f;
	}
	if (t_f!=1.0) {
		puts("Applying temperature factor");
		for (int i=0;i<W_AREA;i++)
			w[i].temp=500+(w[i].temp-500)*t_f;
	}
	if (e_o!=0) {
		puts("Applying elevation offset");
		for (int i=0;i<W_AREA;i++)
			w[i].elev+=e_o;
	}
	if (t_o!=0) {
		puts("Applying temperature offset");
		for (int i=0;i<W_AREA;i++)
			w[i].temp+=t_o;
	}
	puts("Running rivers");
	for (int i=0;i<W_AREA/640;i++)
		run_river(w,rand_land(w));
	puts("Done");
	return w;
}
enum terrain terrain_type(struct worldtile *tile)
{
	short elev=tile->elev;
	short temp=tile->temp;
	enum terrain t=NO_TERRAIN;
	if (elev<200) { // Void
		t=NO_TERRAIN;
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
		else if (elev<700)
			t=SUMMIT;
		else
			t=HIGH_SUMMIT;
	}
	if (temp<400) // Cold
		t--;
	else if (temp>625) // Hot
		t++;
	return t;
}
static inline void putcchar(color_t co,char ch)
{
	set_fg(co);
	putchar(ch);
}
void draw_terrain(enum terrain t)
{
	switch (t) {
	// Nothing
	case COLD_NO_TERRAIN:
	case NO_TERRAIN:
	case HOT_NO_TERRAIN:
		putcchar(BLACK,' ');
		break;
	// Sea
	case COLD_SHALLOW_SEA:
		putcchar(CYAN,'_');
		break;
	case COLD_DEEP_SEA:
		putcchar(TEAL,'_');
		break;
	case SHALLOW_SEA:
	case HOT_SHALLOW_SEA:
		putcchar(LIGHT_BLUE,'_');
		break;
	case DEEP_SEA:
	case HOT_DEEP_SEA:
		putcchar(BLUE,'_');
		break;
	// Sand
	case COLD_BEACH:
		putcchar(WHITE,'~');
		break;
	case BEACH:
	case HOT_BEACH:
		putcchar(YELLOW,'~');
		break;
	// Meadows / Fields
	case COLD_MEADOW:
		putcchar(WHITE,'~');
		break;
	case COLD_FIELD:
		putcchar(LIGHT_GRAY,'~');
		break;
	case MEADOW:
		putcchar(LIGHT_GREEN,'~');
		break;
	case FIELD:
		putcchar(GREEN,'~');
		break;
	case HOT_MEADOW:
		putcchar(YELLOW,'~');
		break;
	case HOT_FIELD:
		putcchar(BROWN,'~');
		break;
	// Forest
	case COLD_FOREST:
		putcchar(WHITE,'%');
		break;
	case FOREST:
		putcchar(GREEN,'%');
		break;
	case HOT_FOREST:
		putcchar(BROWN,'%');
		break;
	// Low Mountain
	case COLD_LOW_MOUNTAIN:
		putcchar(WHITE,'-');
		break;
	case LOW_MOUNTAIN:
		putcchar(DARK_GRAY,'-');
		break;
	case HOT_LOW_MOUNTAIN:
		putcchar(LIGHT_GRAY,'-');
		break;
	// High Mountain
	case COLD_HIGH_MOUNTAIN:
	case HIGH_MOUNTAIN:
		putcchar(WHITE,'=');
		break;
	case HOT_HIGH_MOUNTAIN:
		putcchar(LIGHT_GRAY,'=');
		break;
	// Summit
	case COLD_SUMMIT:
	case SUMMIT:
		putcchar(WHITE,'^');
		break;
	case HOT_SUMMIT:
		putcchar(LIGHT_GRAY,'^');
		break;
	// High Summit
	case COLD_HIGH_SUMMIT:
		putcchar(CYAN,'^');
		break;
	case HIGH_SUMMIT:
		putcchar(WHITE,'^');
		break;
	case HOT_HIGH_SUMMIT:
		putcchar(LIGHT_RED,'^');
		break;
	}
}
char river_char(enum dir d)
{
	switch (d) {
	case SOUTHWEST:
	case NORTHEAST:
		return '/';
	case SOUTH:
	case NORTH:
		return '|';
	case SOUTHEAST:
	case NORTHWEST:
		return '\\';
	case WEST:
	case EAST:
		return '-';
	default:
		return '*';
	}
}
void draw_worldtile(struct worldtile *w)
{
	if (w->faction)
		set_bg(w->faction->color);
	else
		set_bg(BLACK);
	if (w->river) {
		set_fg(w->temp<400?CYAN:BLUE);
		putchar(river_char(w->river));
	} else
		draw_terrain(terrain_type(w));
}
void draw_whole_world(struct worldtile *w)
{
	for (int i=0;i<W_AREA;i++) {
		move_cursor(i%W_WIDTH,i/W_WIDTH);
		draw_worldtile(&w[i]);
	}
}
int rand_land(struct worldtile *w)
{
	int lands[W_AREA],l=0;
	for (int i=0;i<W_AREA;i++)
		if (w[i].elev>=500)
			lands[l++]=i;
	if (!l)
		return 0;
	return lands[rand()%l];
}
