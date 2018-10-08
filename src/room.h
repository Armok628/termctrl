#ifndef ROOM_H
#define ROOM_H
#include "tile.h"
#include "input.h"

void rand_room(struct tile *);
void fix_rooms(struct tile *);
void fix_gaps(struct tile *);
void rand_path(struct tile *);
#endif
