#include "terminal.h"
void sgr(int r)
{
	printf("\033[%dm",r);
}
void mode(gr_t g)
{
	while (*g)
		sgr(*(g++));
}
void move_cursor(int x,int y)
{
	printf("\033[%d;%dH",y+1,x+1);
}
void clear_screen()
{
	sgr(RESET);
	fputs("\033[2J",stdout);
}
void clear_line()
{
	sgr(RESET);
	fputs("\033[2K",stdout);
}
void set_cursor_visible(bool v)
{
	printf("\033[?25%c",v?'h':'l');
}
void set_canon(bool c)
{
	struct termios term;
	tcgetattr(0,&term);
	if (c)
		term.c_lflag|=(ICANON|ECHO);
	else
		term.c_lflag&=~(ICANON|ECHO);
	tcsetattr(0,TCSANOW,&term);
}
void set_blocking(bool b)
{
	struct termios term;
	tcgetattr(0,&term);
	term.c_cc[VMIN]=b;
	tcsetattr(0,TCSANOW,&term);
}
int get_cursor_pos(int xf,int yf)
{
	struct termios term;
	tcgetattr(0,&term);
	set_canon(0);
	set_blocking(1);
	int x,y;
	printf("\033[6n");
	fscanf(stdin,"\033[%d;%dH",&y,&x);
	tcsetattr(0,TCSANOW,&term);
	return (x-1)*xf+(y-1)*yf;
}
