#ifndef ROOM_H
#define ROOM_H
#include "input.h"
#include "tile.h"
enum dir {NORTH,SOUTH,EAST,WEST};

void make_room(struct tile *,int,int,int,int,enum dir);
void random_room(struct tile *);
void fix_rooms(struct tile *);
bool make_path(struct tile *,int);
void random_path(struct tile *);
#endif
