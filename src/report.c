#include "report.h"
int report_height=0;
static int next_line=1;
void vreport(char *fmt,va_list args)
{
	struct entity *e;
	sgr(RESET);
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
	for (int i=1;i<next_line;i++) {
		move_cursor(0,HEIGHT+i);
		clear_line();
	}
	next_line=1;
}
void update_stats(struct entity *e)
{
	move_cursor(0,report_height);
	clear_line();
	report_here("e s d/d s d s d s d s d",e,
			"HP:",e->hp,e->maxhp,
			"RES:",e->res,
			"AGI:",e->agi,
			"WIS:",e->wis,
			"STR:",e->str);
}
