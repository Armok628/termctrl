#include "weather.h"
#define N_PARTICLES 10
void (*weather)(void)=NULL;
void precip(char *chrs,color_t c,int len)
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
			int x=p%Z_WIDTH+z_offset_x,y=p/G_WIDTH+z_offset_y;
			if (x>0&&x<Z_WIDTH&&y>0&&y<Z_HEIGHT) {
				move_cursor(x,y);
				set_fg(c);
				putchar(chrs[rand()%len]);
			}
		}
#else
		int p=rand()%Z_AREA;
		pos[next]=p;
		move_cursor(p%G_WIDTH,p/G_WIDTH);
		set_fg(c);
		putchar(chrs[rand()%len]);
#endif
		if (++next>=N_PARTICLES)
			next=0;
	}
}
void rain(void)
{
	precip("/|\\",BLUE,3);
}
void snow(void)
{
	precip("*.",WHITE,2);
}
