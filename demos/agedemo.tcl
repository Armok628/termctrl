#!/usr/bin/tclsh
set seed [expr {int(rand()*10000000000)}]
for {set i 0} {$i<=9} {incr i} {
	exec ./a.out seed=$seed {*}$argv age=$i >&@ stdout
	if {$i==4} {after 2000}
	after 500
}
