#ifndef ADVANCE_H
#define ADVANCE_H
#include "entity.h"
#include "tile.h"
#include "input.h"
#include "report.h"
extern int player_coords;

void move_entity(struct tile *,int,int);
void take_turn(struct tile *,int);
void advance(struct tile *);
#endif
