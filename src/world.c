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
		pos+=input_offset(d,W_WIDTH);
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
		run_river(w,rand_loc(w,&is_land));
	puts("Placing towns");
	for (int i=0;i<10;i++) {
		int p=rand_loc(w,&is_land);
		w[p].town=true;
		w[p].pop=rand()%(TOWN_POP_CAP/2);
	}
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
void draw_terrain(enum terrain t,color_t bg)
{
	switch (t) {
	// Nothing
	case COLD_NO_TERRAIN:
	case NO_TERRAIN:
	case HOT_NO_TERRAIN:
		draw(' ',BLACK,bg);
		break;
	// Sea
	case COLD_SHALLOW_SEA:
		draw('_',CYAN,bg);
		break;
	case COLD_DEEP_SEA:
		draw('_',TEAL,bg);
		break;
	case SHALLOW_SEA:
	case HOT_SHALLOW_SEA:
		draw('_',LIGHT_BLUE,bg);
		break;
	case DEEP_SEA:
	case HOT_DEEP_SEA:
		draw('_',BLUE,bg);
		break;
	// Sand
	case COLD_BEACH:
		draw('~',WHITE,bg);
		break;
	case BEACH:
	case HOT_BEACH:
		draw('~',YELLOW,bg);
		break;
	// Meadows / Fields
	case COLD_MEADOW:
		draw('~',WHITE,bg);
		break;
	case COLD_FIELD:
		draw('~',LIGHT_GRAY,bg);
		break;
	case MEADOW:
		draw('~',LIGHT_GREEN,bg);
		break;
	case FIELD:
		draw('~',GREEN,bg);
		break;
	case HOT_MEADOW:
		draw('~',YELLOW,bg);
		break;
	case HOT_FIELD:
		draw('~',BROWN,bg);
		break;
	// Forest
	case COLD_FOREST:
		draw('%',WHITE,bg);
		break;
	case FOREST:
		draw('%',GREEN,bg);
		break;
	case HOT_FOREST:
		draw('%',BROWN,bg);
		break;
	// Low Mountain
	case COLD_LOW_MOUNTAIN:
		draw('-',WHITE,bg);
		break;
	case LOW_MOUNTAIN:
		draw('-',DARK_GRAY,bg);
		break;
	case HOT_LOW_MOUNTAIN:
		draw('-',LIGHT_GRAY,bg);
		break;
	// High Mountain
	case COLD_HIGH_MOUNTAIN:
	case HIGH_MOUNTAIN:
		draw('=',WHITE,bg);
		break;
	case HOT_HIGH_MOUNTAIN:
		draw('=',LIGHT_GRAY,bg);
		break;
	// Summit
	case COLD_SUMMIT:
	case SUMMIT:
		draw('^',WHITE,bg);
		break;
	case HOT_SUMMIT:
		draw('^',LIGHT_GRAY,bg);
		break;
	// High Summit
	case COLD_HIGH_SUMMIT:
		draw('^',CYAN,bg);
		break;
	case HIGH_SUMMIT:
		draw('^',WHITE,bg);
		break;
	case HOT_HIGH_SUMMIT:
		draw('^',LIGHT_RED,bg);
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
	color_t bg=w->faction?w->faction->color:BLACK;
	if (w->town) {
		set_fg(LIGHT_RED);
		if (w->pop<1)
			draw('_',LIGHT_RED,bg);
		else if (w->pop<TOWN_POP_CAP/4)
			draw('.',LIGHT_RED,bg);
		else if (w->pop<TOWN_POP_CAP/2)
			draw(':',LIGHT_RED,bg);
		else if (w->pop<TOWN_POP_CAP*3/4)
			draw('*',LIGHT_RED,bg);
		else
			draw('#',LIGHT_RED,bg);
	} else if (w->river) {
		draw(river_char(w->river),w->temp<400?CYAN:LIGHT_BLUE,bg);
	} else
		draw_terrain(terrain_type(w),bg);
}
void draw_whole_world(struct worldtile *w)
{
	for (int i=0;i<W_AREA;i++) {
		next_draw(i%W_WIDTH,i/W_WIDTH);
		draw_worldtile(&w[i]);
	}
}
int rand_loc(struct worldtile *w,bool (*f)(struct worldtile *,int))
{
	int t[W_AREA],n=0;
	for (int i=0;i<W_AREA;i++)
		if (f(w,i))
			t[n++]=i;
	if (!n)
		return 0;
	return t[rand()%n];
}
bool is_land(struct worldtile *w,int i)
{
	return w[i].elev>=500;
}
