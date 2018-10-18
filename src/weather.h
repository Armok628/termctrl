#ifndef WEATHER_H
#define WEATHER_H
#include <time.h>
#include "color.h"
#include "advance.h"
extern void (*weather)(void);

void rain(void);
void snow(void);
#endif
