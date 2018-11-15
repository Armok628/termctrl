#include "target.h"
int target(struct tile *z,int pos)
{
	int p=pos;
	char c;
	for (;;) {
#ifdef SCROLL
		next_draw(z_offset_x+p%Z_WIDTH,z_offset_y+p/Z_WIDTH);
#else
		next_draw(p%Z_WIDTH,p/Z_WIDTH);
#endif
		draw('X',LIGHT_RED,BLACK);
		c=fgetc(stdin);
		if (c=='\n')
			break;
		int to=p+input_offset(c);
		if (legal_move(p,to)) {
			draw_pos(z,p);
			p=to;
		}
#ifdef SCROLL
		scroll_zone(p);
		draw_zone(z);
#endif
	}
#ifdef SCROLL
	scroll_zone(pos);
	draw_zone(z);
#else
	draw_pos(z,p);
#endif
	return p;
}
