#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#include <time.h>
#include "tile.h"
#include "advance.h"
extern bool exit_req;

char key(void);
int input_offset_width(char,int);
int input_offset(char);
#endif
