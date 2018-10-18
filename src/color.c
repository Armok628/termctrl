#include "color.h"
color_t (*colormod)(color_t)=NULL;
void set_fg(color_t c)
{
	if (colormod)
		c=colormod(c);
	printf("\033[%hhu;%hhum",
			(unsigned char)(c>>7?BOLD:NORMAL),
			(unsigned char)(c&~(1<<7)));
}
void set_bg(color_t c)
{ // BOLD bit ignored; no standard bold background colors
	printf("\033[%hhum",
			(unsigned char)((10+c)&~(1<<7)));
}
color_t dark(color_t c)
{
	return c>>7?LIGHT_GRAY:DARK_GRAY;
}
color_t cold(color_t c)
{
	return c>>7?BLUE:TEAL;
}
