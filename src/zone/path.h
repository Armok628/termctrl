#ifndef PATH_H
#define PATH_H
#include <stdio.h>
#include "room.h"

bool plan_path(struct tile *,int,int);
char go_to(struct tile *,int,int);
#endif
