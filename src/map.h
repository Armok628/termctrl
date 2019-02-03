#ifndef MAP_H
#define MAP_H
#include <time.h>
#include "colors.h"
#include "world.h"
#include "report.h"
#include "sort.h"
extern int world_pos;

void scroll_map(int);
void draw_world_pos(struct worldtile *,int);
void open_map(struct worldtile *);
bool legal_world_move(int,int);
#endif
