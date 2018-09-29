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
	static int offsets[10]={0,
		WIDTH-1,WIDTH,WIDTH+1,
		-1,0,1,
		-WIDTH-1,-WIDTH,-WIDTH+1};
	if ('0'<=c&&c<='9')
		return offsets[c-'0'];
	return 0;
}
