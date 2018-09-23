#include <stdio.h>
#include <stdbool.h>
#include "src/terminal.h"
int main(int argc,char **argv)
{
	clear_screen();
	move_cursor(10,10);
	sgr(FG_GREEN);
	sgr(BG_BLUE);
	fputs("TESTING TESTING",stdout);
	sgr(RESET);
	move_cursor(80,24);
	putchar('#');
	move_cursor(0,30);
	set_canon(false);
	printf("^D to quit\n");
	char input=fgetc(stdin);
	while (input!=4) {
		printf("%d\n",input);
		input=fgetc(stdin);
	}
	set_canon(true);
	return 0;
}
