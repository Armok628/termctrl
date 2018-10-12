#include "target.h"
int target(struct tile *z,int pos)
{
	int p=pos;
	char c;
	for (;;) {
#ifdef SCROLL_ZONE
		move_cursor(x_offset+p%Z_WIDTH,y_offset+p/Z_WIDTH);
#else
		move_cursor(p%Z_WIDTH,p/Z_WIDTH);
#endif
		set_fg(LIGHT_RED);
		set_bg(BLACK);
		putchar('X');
		c=fgetc(stdin);
		if (c=='\n')
			break;
		int to=p+input_offset(c);
		if (legal_move(p,to)) {
			draw_pos(z,p);
			p=to;
		}
#ifdef SCROLL_ZONE
		scroll_to(p);
		draw_zone(z);
#endif
	}
#ifdef SCROLL_ZONE
	scroll_to(pos);
	draw_zone(z);
#else
	draw_pos(z,p);
#endif
	return p;
}
