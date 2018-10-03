#include "report.h"
int report_height=HEIGHT;
static int line=1;
void report(char *fmt,...)
{
	va_list args;
	struct entity *e;
	va_start(args,fmt);
	sgr(RESET);
	move_cursor(0,HEIGHT+line);
	clear_line();
	for (char *s=fmt;*s;s++) {
		switch (*s) {
		case 's':
			fputs(va_arg(args,char *),stdout);
			break;
		case 'd':
			printf("%d",va_arg(args,int));
			break;
		case 'c':
			putchar(va_arg(args,int));
			break;
		case 'e':
			e=va_arg(args,struct entity *);
			printf("%s (",e->name);
			draw_entity(e);
			sgr(RESET);
			putchar(')');
			break;
		default:
			putchar(*s);
			break;
		}
	}
	va_end(args);
	line++;
}
void clear_reports(void)
{
	for (int i=0;i<line;i++) {
		move_cursor(0,HEIGHT+i);
		clear_line();
	}
	line=1;
}
