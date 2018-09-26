#include "tile.h"
static inline void sgr2(char *arr)
{
	sgr(arr[0]);
	if (arr[1])
		sgr(arr[1]);
}
void draw_tile(struct tile *t)
{
	sgr(RESET);
	sgr(BG_BLACK);
	if (t->e) {
		sgr2(t->e->gr);
		putchar(t->e->sym);
	} else if (t->fg_sym) {
		sgr2(t->fg_gr);
		putchar(t->fg_sym);
	} else if (t->bg_sym) {
		sgr2(t->bg_gr);
		putchar(t->bg_sym);
	}
}
