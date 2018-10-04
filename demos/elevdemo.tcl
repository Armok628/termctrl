#!/usr/bin/tclsh
set seed [expr {int(rand()*1e10)}]
for {set i -200} {$i<=200} {incr i 20} {
	exec ./a.out seed=$seed {*}$argv e_o=$i >&@ stdout
	if {$i==0} {after 2000}
	after 500
}
