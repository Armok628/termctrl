#ifndef REPORT_H
#define REPORT_H
#include <stdio.h>
#include <stdarg.h>
#include "tile.h"
#include "entity.h"
#include "terminal.h"

void report(char *,...);
void clear_reports(void);
#endif
