#include "target.h"
int target(struct tile *z,int pos)
{
	char c;
	for (;;) {
		move_cursor(pos%WIDTH,pos/WIDTH);
		set_fg(LIGHT_RED);
		set_bg(BLACK);
		putchar('X');
		c=fgetc(stdin);
		if (c=='\n')
			break;
		int to=pos+input_offset(c);
		if (legal_move(pos,to)) {
			draw_pos(z,pos);
			pos=to;
		}
	}
	draw_pos(z,pos);
	return pos;
}
