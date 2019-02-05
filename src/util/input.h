#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#include <stdbool.h>
#include <curses.h>
extern bool exit_req;

int key(void);
int input_offset(char,int);
#endif
