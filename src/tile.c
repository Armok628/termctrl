#include "tile.h"
void draw_tile(struct tile *t)
{
	sgr(RESET);
	if (t->e) {
		sgr(t->e->mode[0]);
		sgr(t->e->mode[1]);
		putchar(t->e->sym);
	} else if (t->fg_sym) {
		sgr(t->fg_mode[0]);
		sgr(t->fg_mode[1]);
		putchar(t->fg_sym);
	} else if (t->bg_sym) {
		sgr(t->bg_mode[0]);
		sgr(t->bg_mode[1]);
		putchar(t->bg_sym);
	}
}
