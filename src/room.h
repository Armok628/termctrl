#ifndef ROOM_H
#define ROOM_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sort.h"
#define WIDTH 100
#define HEIGHT 40
#define AREA (WIDTH*HEIGHT)

void print_area(char *);
void make_room(char *,int,int,int,int);
void place_doors(char *,int,int,int,int);
void partition_room_vert(char *,int,int,int,int);
void partition_room_horiz(char *,int,int,int,int);
void partition_room(char *,int,int,int,int);
#endif
