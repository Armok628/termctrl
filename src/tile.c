#include "tile.h"
void draw_tile(struct tile *t)
{
	sgr(RESET);
	if (t->e) {
		mode(t->e->mode);
		putchar(t->e->sym);
	} else if (t->fg_sym) {
		mode(t->fg_mode);
		putchar(t->fg_sym);
	} else if (t->bg_sym) {
		mode(t->bg_mode);
		putchar(t->bg_sym);
	}
}
