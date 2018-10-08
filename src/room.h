#ifndef ROOM_H
#define ROOM_H
#include "tile.h"

void room(struct tile *,int,int,int,int,int);
void rand_room(struct tile *);
void fix_rooms(struct tile *);
void fix_gaps(struct tile *);
#endif
