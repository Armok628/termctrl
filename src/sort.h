#ifndef SORT_H
#define SORT_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void sort(int *,int,bool (*)(int,int));
bool ascending(int,int);
bool descending(int,int);
#endif
