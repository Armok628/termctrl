#ifndef ROOM_H
#define ROOM_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // Temporary
#include "sort.h"
#include "zone.h"

void make_room(char *,int,int,int,int);
void partition_room(char *,int,int,int,int);
void place_doors(char *);
void random_room(char *);
void print_area(char *);
#endif
