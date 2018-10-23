#include "map.h"
int world_pos=-1;
#ifdef SCROLL
int w_offset_x=0;
int w_offset_y=0;
void scroll_map(int pos)
{
	w_offset_x=G_WIDTH/2-pos%W_WIDTH;
	w_offset_y=G_HEIGHT/2-pos/W_WIDTH;
}
#endif
void draw_world_pos(struct worldtile *w,int pos)
{
	int x=pos%W_WIDTH,y=pos/W_WIDTH;
#ifdef SCROLL
	x+=w_offset_x;
	y+=w_offset_y;
	if (x>=0&&x<G_WIDTH&&y>=0&&y<G_HEIGHT) // If x,y is on-screen
#endif
	{
		move_cursor(x,y);
		draw_worldtile(&w[pos]);
	}
}
void draw_world(struct worldtile *w)
{
#ifdef SCROLL
	for (int x=0;x<G_WIDTH;x++)
		for (int y=0;y<G_HEIGHT;y++) {
			int x2=x-w_offset_x,y2=y-w_offset_y;
			int i=x2+y2*W_WIDTH;
			move_cursor(x,y);
			if (x2<0||x2>=W_WIDTH||y2<0||y2>=W_HEIGHT) {
				sgr(RESET);
				putchar(' ');
			} else
				draw_worldtile(&w[i]);
		}
#else
	for (int i=0;i<W_AREA;i++)
		draw_world_pos(w,i);
#endif
}
bool legal_world_move(int from,int to)
{
	int dx=to%W_WIDTH-from%W_WIDTH;
	return -1<=dx&&dx<=1&&0<=to&&to<W_AREA;
}
/*Temporary*/
struct faction f1={
	.name="Faction 1",
	.color=GREEN,
};
struct faction f2={
	.name="Faction 2",
	.color=WHITE,
};
/**/
void open_map(struct worldtile *w)
{
	clear_screen();
#ifndef SCROLL
	report_height=W_HEIGHT;
#endif
	draw_world(w);
	while (/**/!exit_req/**/) {
#ifdef SCROLL
		scroll_map(world_pos);
		draw_world(w);
		int x=world_pos%W_WIDTH+w_offset_x;
		int y=world_pos/W_WIDTH+w_offset_y;
		move_cursor(x,y);
#else
		move_cursor(world_pos%W_WIDTH,world_pos/W_WIDTH);
#endif
		set_fg(RED);
		set_bg(BLACK);
		putchar('*');
		char c=key();
		draw_world_pos(w,world_pos);
		int to=world_pos+input_offset_width(c,W_WIDTH);
		if (legal_world_move(world_pos,to))
			world_pos=to;
		/**/
		if (c=='\n') {
			static bool f=false;
			w[world_pos].faction=f?&f1:&f2;
			f=!f;
		}
		if (c==' ') {
			for (int i=0;i<10;i++)
				spread_factions(w);
			draw_world(w);
		}
		/**/
	}
}

