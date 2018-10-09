#include <stdio.h>
#include <string.h>
#include "src/terminal.h"
const int max=1<<16;
int main(int argc,char **argv)
{
	FILE *out=stdout;
	if (argc>1) {
		if (!strcmp(argv[1],"stderr"))
			out=stderr;
		else
			out=fopen(argv[1],"w");
		if (!out)
			out=stdout;
	}
	set_cursor_visible(false);
	int old=get_cursor_pos(1,max);
	move_cursor(max,max);
	int size=get_cursor_pos(1,max);
	move_cursor(old%max,old/max);
	fprintf(out,"%d %d",1+size%max,1+size/max);
	putchar('\n');
	set_cursor_visible(true);
	return 0;
}
