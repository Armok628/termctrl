#include "input.h"
#define KEY_EOF 4
bool exit_req=false;
int key(void)
{
	char c=getch();
	exit_req=c==KEY_EOF||c=='q';
	return c;
}
int input_offset(char c,int w)
{
	switch (c) {
	case '1':
	case 'b':
		return w-1;
	case '2':
	case 'j':
		return w;
	case '3':
	case 'n':
		return w+1;
	case '4':
	case 'h':
		return -1;
	case '6':
	case 'l':
		return 1;
	case '7':
	case 'y':
		return -w-1;
	case '8':
	case 'k':
		return -w;
	case '9':
	case 'u':
		return -w+1;
	default:
		return 0;
	}
}
