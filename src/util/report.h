#ifndef REPORT_H
#define REPORT_H
#include <stdio.h>
#include <stdarg.h>
#include "colors.h"
extern int report_height;

void next_report(void);
void report_here(char *,...);
void report(char *,...);
void clear_reports(void);
#endif
