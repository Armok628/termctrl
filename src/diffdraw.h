#ifndef DIFFDRAW_H
#define DIFFDRAW_H
#include "terminal.h"
#include "color.h"
#ifndef TERM_WIDTH
#define TERM_WIDTH 80
#endif
#ifndef TERM_HEIGHT
#define TERM_HEIGHT 24
#endif
#define TERM_AREA (TERM_WIDTH*TERM_HEIGHT)

void next_draw(int,int);
void draw(char,color_t,color_t);
void redraw(void);
#endif
