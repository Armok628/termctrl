#ifndef SORT_H
#define SORT_H
#include <stdlib.h>
#include <stdbool.h>

void swap(int *,int *);
void insertion_sort(int *,int,bool (*)(int,int));
bool ascending(int,int);
bool descending(int,int);
void reverse(int *,int);
void rand_fixed_sum(int *,int,int);
#endif
