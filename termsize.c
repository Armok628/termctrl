#include "src/terminal.h"
const int max=1<<16;
int main(/*int argc,char **argv*/)
{
	set_cursor_visible(false);
	int old=get_cursor_pos(1,max);
	move_cursor(max,max);
	int size=get_cursor_pos(1,max);
	move_cursor(old%(max),old/(max));
	printf("%d %d\n",1+size%max,1+size/max);
	set_cursor_visible(true);
	return 0;
}
