#include "tile.h"
void draw_tile(struct tile *t)
{
	sgr(RESET);
	sgr(BG_BLACK);
	if (t->e) {
		sgr(t->e->gr[0]);
		sgr(t->e->gr[1]);
		putchar(t->e->sym);
	} else if (t->fg_sym) {
		sgr(t->fg_gr[0]);
		sgr(t->fg_gr[1]);
		putchar(t->fg_sym);
	} else if (t->bg_sym) {
		sgr(t->bg_gr[0]);
		sgr(t->bg_gr[1]);
		putchar(t->bg_sym);
	}
}
