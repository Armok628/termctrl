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
		int i=rand()%G_AREA;
		pos[next]=i;
		move_cursor(i%G_WIDTH,i/G_WIDTH);
		set_fg(BLUE);
		putchar(drops[rand()%3]);
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
		int i=rand()%G_AREA;
		pos[next]=i;
		move_cursor(i%G_WIDTH,i/G_WIDTH);
		set_fg(WHITE);
		putchar(rand()%2?'*':'.');
		if (++next>=N_PARTICLES)
			next=0;
	}
}

