#ifndef MAP_H
#define MAP_H
#include <time.h>
#include "world.h"
#include "report.h"
#include "sort.h"
#include "diffdraw.h"
extern int world_pos;

#ifdef SCROLL
void scroll_map(int);
#endif
void draw_world_pos(struct worldtile *,int);
void open_map(struct worldtile *);
bool legal_world_move(int,int);
#endif
