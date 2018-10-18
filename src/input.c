#include "input.h"
bool exit_req=false;
/**/void snow(void);
char key(void)
{
	char c='\0';
	if (!weather)
		c=fgetc(stdin);
	else {
		set_blocking(false);
		while (c<1) {
			c=fgetc(stdin);
			weather();
		}
		set_blocking(true);
	}
	exit_req=c==4||c=='q';
	return c;
}
int input_offset_width(char c,int w)
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
int input_offset(char c)
{
	return input_offset_width(c,Z_WIDTH);
}
