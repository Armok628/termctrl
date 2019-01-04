#ifndef ROOM_H
#define ROOM_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // Temporary
#include "sort.h"
#include "zone.h"

bool is_wall(struct tile *);
void make_room(struct tile *,int,int,int,int);
void partition_room(struct tile *,int,int,int,int);
void place_doors(struct tile *);
void random_room(struct tile *);
#endif
