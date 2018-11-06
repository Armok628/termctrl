#!/usr/bin/tclsh
lassign $argv w h
if {$w eq "max" || $h eq ""} {
	exec make size
	exec ./a.out size >@ stdout
	lassign [split [read [open size r]] " "] w h
	file delete size
	incr h -2
}
puts "make world 'CFLAGS=-O3 -DW_WIDTH=$w -DW_HEIGHT=$h'"
exec  make world "CFLAGS=-O3 -DW_WIDTH=$w -DW_HEIGHT=$h"
