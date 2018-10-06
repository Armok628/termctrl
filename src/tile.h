#ifndef TILE_H
#define TILE_H
#include "terminal.h"
#include "entity.h"
#ifndef WIDTH
#define WIDTH 80
#endif
#ifndef HEIGHT
#define HEIGHT 24
#endif
#define AREA (WIDTH*HEIGHT)
struct tile {
	struct entity *e,*c;
	char fg,bg;
	color_t fg_c,bg_c;
};

void draw_tile(struct tile *);
struct tile *new_zone(struct tile *);
void free_zone(struct tile *);
void draw_pos(struct tile *,int);
void draw_zone(struct tile *);
bool legal_move(int,int);
#endif
