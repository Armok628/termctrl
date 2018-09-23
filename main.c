#include <stdio.h>
#include <stdbool.h>
#include "src/terminal.h"
int main(int argc,char **argv)
{
	clear_screen();
	int x=80,y=24;
	set_canon(false);
	set_cursor_visible(false);
	move_cursor(80,24);
	putchar('@');
	char input=fgetc(stdin);
	while (input!='q'&&input!=4) {
		move_cursor(x,y);
		putchar(' ');
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
		move_cursor(x,y);
		putchar('@');
		input=fgetc(stdin);
	}
	set_canon(true);
	set_cursor_visible(true);
	move_cursor(0,25);
	return 0;
}
