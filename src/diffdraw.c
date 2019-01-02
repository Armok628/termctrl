#include "diffdraw.h"
static char screen_chrs[TERM_AREA];
static color_t screen_fgs[TERM_AREA];
static color_t screen_bgs[TERM_AREA];
static int cursor_x=0,cursor_y=0;
void next_draw(int x,int y)
{
	cursor_x=x;
	cursor_y=y;
}
void draw(char c,color_t fg,color_t bg)
{
	bool r=false;
	int p=cursor_x+cursor_y*TERM_WIDTH;
	r|=c!=screen_chrs[p];
	r|=fg!=screen_fgs[p];
	r|=bg!=screen_bgs[p];
	screen_chrs[p]=c;
	screen_fgs[p]=fg;
	screen_bgs[p]=bg;
	if (r) {
		move_cursor(cursor_x,cursor_y);
		set_fg(fg);
		set_bg(bg);
		putchar(c);
	}
}
void redraw(void)
{
	sgr(RESET);
	clear_screen();
	for (int i=0;i<TERM_AREA;i++) {
		move_cursor(i%TERM_WIDTH,i/TERM_WIDTH);
		if (screen_chrs[i]) {
			set_fg(screen_fgs[i]);
			set_bg(screen_bgs[i]);
			putchar(screen_chrs[i]);
		} else {
			sgr(RESET);
			putchar(' ');
		}
	}
}
