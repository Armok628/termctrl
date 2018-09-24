#include <stdio.h>
#include <stdbool.h>
#include "src/terminal.h"
int main(int argc,char **argv)
{
	gr_t p={FG_GREEN,BG_GREEN,ITALIC,BOLD,STRIKETHROUGH,UNDERLINE};
	gr_t n={BOLD,FG_YELLOW,BG_BLUE};
	clear_screen();
	int x=80,y=24;
	set_canon(false);
	set_cursor_visible(false);
	char input='\0';
	while (input!=4) {
		move_cursor(x,y);
		mode(p);
		putchar('@');
		input=fgetc(stdin);
		move_cursor(x,y);
		sgr(RESET);
		putchar(' ');
		move_cursor(0,25);
		clear_line();
		mode(n);
		printf("%d",input);
		switch (input) {
		case 'h':
			x--;
			break;
		case 'j':
			y++;
			break;
		case 'k':
			y--;
			break;
		case 'l':
			x++;
		}
	}
	set_canon(true);
	set_cursor_visible(true);
	move_cursor(0,26);
	sgr(RESET);
	return 0;
}
