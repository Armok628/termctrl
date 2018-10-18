#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#include "tile.h"
#ifndef NO_WEATHER
#include "weather.h"
#endif
extern bool exit_req;

char key(void);
int input_offset_width(char,int);
int input_offset(char);
#endif
