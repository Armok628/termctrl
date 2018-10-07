#ifndef COMBAT_H
#define COMBAT_H
#include "advance.h"

int max_damage(struct entity *,struct entity *);
bool attack(struct tile *,int,int);
#endif
