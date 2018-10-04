#ifndef REPORT_H
#define REPORT_H
#include <stdio.h>
#include <stdarg.h>
#include "tile.h"
#include "entity.h"
#include "terminal.h"
extern int report_height;

void report(char *,...);
void clear_reports(void);
void update_stats(struct entity *);
#endif