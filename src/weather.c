#include "weather.h"
#define N_PARTICLES (Z_AREA/192)
void (*weather)(void)=NULL;
void precip(char c,color_t fg)
{
	static int pos[N_PARTICLES]={0},next=0;
	static clock_t last_update=0;
	clock_t now=clock();
	if (now-last_update>10000) {
		last_update=now;
		draw_pos(current_zone,pos[next]);
#ifdef SCROLL
		pos[next]=rand()%Z_AREA;
		for (int i=0;i<N_PARTICLES;i++) {
			int p=pos[i];
			int x=p%Z_WIDTH+z_offset_x,y=p/Z_WIDTH+z_offset_y;
			if (x>0&&x<G_WIDTH&&y>0&&y<G_HEIGHT) {
				next_draw(x,y);
				draw(c,fg,BLACK);
			}
		}
#else
		int p=rand()%Z_AREA;
		pos[next]=p;
		next_draw(p%Z_WIDTH,p/Z_WIDTH);
		set_fg(c);
		draw(c,fg,BLACK);
#endif
		if (++next>=N_PARTICLES)
			next=0;
	}
}
void rain(void)
{
	precip(',',LIGHT_BLUE);
}
void snow(void)
{
	precip('*',WHITE);
}
