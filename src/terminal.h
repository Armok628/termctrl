#ifndef TERMINAL_H
#define TERMINAL_H
#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
enum gr { // "graphics rendition"
	RESET,BOLD,FAINT,ITALIC,UNDERLINE,
	REVERSE=7,CONCEAL,STRIKETHROUGH,
	NO_BOLD=21,NORMAL,NO_ITALIC,NO_UNDERLINE,NO_BLINK,NO_REVERSE,NO_CONCEAL,
	FG_BLACK=30,FG_RED,FG_GREEN,FG_YELLOW,
	FG_BLUE,FG_MAGENTA,FG_CYAN,FG_GRAY,
	FG_WHITE,FG_DEFAULT,
	BG_BLACK,BG_RED,BG_GREEN,BG_YELLOW,
	BG_BLUE,BG_MAGENTA,BG_CYAN,BG_GRAY,
	BG_WHITE,BG_DEFAULT,
	AIX_FG_BLACK=90,AIX_FG_RED,AIX_FG_GREEN,AIX_FG_ORANGE,
	AIX_FG_BLUE,AIX_FG_MAGENTA,AIX_FG_CYAN,AIX_FG_GRAY,
	AIX_FG_WHITE,AIX_FG_DEFAULT,
	AIX_BG_BLACK,AIX_BG_RED,AIX_BG_GREEN,AIX_BG_ORANGE,
	AIX_BG_BLUE,AIX_BG_MAGENTA,AIX_BG_CYAN,AIX_BG_GRAY,
	AIX_BG_WHITE,AIX_BG_DEFAULT,
}; // All codes < 255, so safe to store as char

void sgr(int); // "set graphics rendition"
void move_cursor(int,int);
void clear_screen();
void clear_line();
void set_cursor_visible(bool);
void set_canon(bool);
void set_blocking(bool);
int get_cursor_pos(int,int);
#endif
