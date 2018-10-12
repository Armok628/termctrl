#ifndef TILE_H
#define TILE_H
#include "terminal.h"
#include "entity.h"
#ifndef Z_WIDTH
#define Z_WIDTH 80
#endif
#ifndef Z_HEIGHT
#define Z_HEIGHT 24
#endif
#ifndef G_WIDTH
#define G_WIDTH 80
#endif
#ifndef G_HEIGHT
#define G_HEIGHT 24
#endif
#define AREA (Z_WIDTH*Z_HEIGHT)
struct tile {
	struct entity *e,*c;
	char fg,bg;
	color_t fg_c,bg_c;
};
#include "room.h"
extern int x_offset,y_offset;

void draw_tile(struct tile *);
struct tile *new_zone(struct tile *);
void free_zone(struct tile *);
void draw_pos(struct tile *,int);
void draw_zone(struct tile *);
bool legal_move(int,int);
#endif
