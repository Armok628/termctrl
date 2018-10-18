#include "weather.h"
#define N_PARTICLES 10
void (*weather)(void)=NULL;
void rain(void)
{
	static int pos[N_PARTICLES]={0},next=0;
	static char drops[3]="/|\\";
	static clock_t last_update=0;
	clock_t now=clock();
	if (now-last_update>10000) {
		last_update=now;
		draw_pos(current_zone,pos[next]);
		int i=rand()%Z_AREA;
		pos[next]=i;
#ifdef SCROLL
		int x=i%Z_WIDTH+z_offset_x,y=i/G_WIDTH+z_offset_y;
		if (x>0&&x<Z_WIDTH&&y>0&&y<Z_HEIGHT) {
			move_cursor(x,y);
			set_fg(BLUE);
			putchar(drops[rand()%3]);
		}
#else
		move_cursor(i%G_WIDTH,i/G_WIDTH);
		set_fg(BLUE);
		putchar(drops[rand()%3]);
#endif
		if (++next>=N_PARTICLES)
			next=0;
	}
}
void snow(void)
{
	static int pos[N_PARTICLES]={0},next=0;
	static clock_t last_update=0;
	clock_t now=clock();
	if (now-last_update>10000) {
		last_update=now;
		draw_pos(current_zone,pos[next]);
		int i=rand()%Z_AREA;
		pos[next]=i;
#ifdef SCROLL
		int x=i%Z_WIDTH+z_offset_x,y=i/G_WIDTH+z_offset_y;
		if (x>0&&x<Z_WIDTH&&y>0&&y<Z_HEIGHT) {
			move_cursor(x,y);
			set_fg(WHITE);
			putchar(rand()%2?'*':'.');
		}
#else
		move_cursor(i%G_WIDTH,i/G_WIDTH);
		set_fg(WHITE);
		putchar(rand()%2?'*':'.');
#endif
		if (++next>=N_PARTICLES)
			next=0;
	}
}
