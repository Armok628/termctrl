#include "report.h"
int report_height=0;
static int next_line=1;
void vreport(char *fmt,va_list args)
{
	char *str;
	for (char *s=fmt;*s;s++) {
		if (*s!='%') {
			addch(*s);
			continue;
		}
		s++;
		switch (*s) {
		case 's':
			str=va_arg(args,char *);
			if (str)
				printw(str);
			else
				printw("(null)");
			break;
		case 'd':
			printw("%d",va_arg(args,int));
			break;
		case 'f':
			printw("%f",va_arg(args,double));
			break;
		case 'c':
			addch(va_arg(args,int));
			break;
		default:
			addch(*s);
			break;
		}
	}
}
void next_report(void)
{
	move(report_height+next_line++,0);
	clrtoeol();
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
		move(report_height+i,0);
		clrtoeol();
	}
	next_line=1;
}

