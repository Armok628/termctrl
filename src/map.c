#include "map.h"
int world_pos=-1;
int w_offset_x=0;
int w_offset_y=0;
#ifdef SCROLL
void scroll_map(int pos)
{
	w_offset_x=G_WIDTH/2-pos%W_WIDTH;
	w_offset_y=G_HEIGHT/2-pos/W_WIDTH;
}
#endif
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
	for (int i=0;i<W_AREA;i++) {
		move_cursor(i%W_WIDTH,i/W_WIDTH);
		draw_worldtile(&w[i]);
	}
#endif
}
void open_map(struct worldtile *w)
{
	clear_screen();
#ifndef SCROLL
	report_height=W_HEIGHT;
#endif
	draw_world(w);
	/*for (???)*/ {
		int x=world_pos%W_WIDTH+w_offset_x;
		int y=world_pos/W_WIDTH+w_offset_y;
		move_cursor(x,y);
		set_fg(RED);
		set_bg(BLACK);
		putchar('*');
	}
}

