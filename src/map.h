#ifndef MAP_H
#define MAP_H
#include "world.h"
#include "tile.h"
#include "report.h"
extern int world_pos;

#ifdef SCROLL
void scroll_map(int);
#endif
void open_map(struct worldtile *);
#endif
