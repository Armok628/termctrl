#ifndef RANGE_H
#define RANGE_H
#include <stdlib.h>
typedef int range_t[2];
static inline int rrand(range_t r)
{
	return r[0]+rand()%(r[1]-r[0]+1);
}
#endif
