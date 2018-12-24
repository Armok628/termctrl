#include "report.h"
int report_height=0;
static int next_line=1;
void vreport(char *fmt,va_list args)
{
	char *str;
	sgr(RESET);
	for (char *s=fmt;*s;s++) {
		if (*s!='%') {
			putchar(*s);
			continue;
		}
		s++;
		switch (*s) {
		case 's':
			str=va_arg(args,char *);
			if (str)
				fputs(str,stdout);
			else
				fputs("(null)",stdout);
			break;
		case 'd':
			printf("%d",va_arg(args,int));
			break;
		case 'f':
			printf("%f",va_arg(args,double));
			break;
		case 'c':
			putchar(va_arg(args,int));
			break;
		default:
			putchar(*s);
			break;
		}
	}
}
void next_report(void)
{
	move_cursor(0,report_height+next_line++);
	clear_line();
}
void report(char *fmt,...)
{
	va_list args;
	va_start(args,fmt);
	next_report();
	vreport(fmt,args);
	va_end(args);
}
void report_here(char *fmt,...)
{
	va_list args;
	va_start(args,fmt);
	vreport(fmt,args);
	va_end(args);
}
void clear_reports(void)
{
	for (int i=0;i<next_line;i++) {
		move_cursor(0,report_height+i);
		clear_line();
	}
	next_line=1;
}

