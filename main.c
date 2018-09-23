#include <stdio.h>
#include <stdbool.h>
#include "src/terminal.h"
int main(int argc,char **argv)
{
	clear_screen();
	int x=80,y=24;
	set_canon(false);
	set_cursor_visible(false);
	char input='\0';
	while (input!=4) {
		move_cursor(x,y);
		putchar('@');
		input=fgetc(stdin);
		move_cursor(x,y);
		putchar(' ');
		move_cursor(0,25);
		clear_line();
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
	return 0;
}
