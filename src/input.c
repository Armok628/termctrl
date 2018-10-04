#include "input.h"
bool exit_req=false;
char key(void)
{
	char c=fgetc(stdin);
	exit_req=c==4;
	return c;
}
int input_offset(char c)
{
	switch (c) {
	case '1':
	case 'b':
		return WIDTH-1;
	case '2':
	case 'j':
		return WIDTH;
	case '3':
	case 'n':
		return WIDTH+1;
	case '4':
	case 'h':
		return -1;
	case '6':
	case 'l':
		return 1;
	case '7':
	case 'y':
		return -WIDTH-1;
	case '8':
	case 'k':
		return -WIDTH;
	case '9':
	case 'u':
		return -WIDTH+1;
	default:
		return 0;
	}
}
