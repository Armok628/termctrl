#include "colors.h"
int color(short f,short b)
{
	static char pairs[(1<<6)+1];
	register bool bold=f&BOLD;
	f&=BOLD-1;
	b&=BOLD-1;
	int i=((f<<3)|b)+1;
	// ^ Adding 1 to i prevents color(BLACK,BLACK) => i=0
	if (!pairs[i])
		init_pair(i,f,b);
	return (bold?A_BOLD:0)|COLOR_PAIR(i);
}
void init_rogue(void)
{
	initscr();
	start_color();
	keypad(stdscr,true);
	curs_set(0);
	noecho();
	raw();
	clear();
	use_default_colors();
}
