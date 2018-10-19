#!/usr/bin/tclsh
set seed [expr {int(rand()*1e10)}]
for {set i 500} {$i>=-500} {incr i -50} {
	exec ./a.out seed=$seed {*}$argv t_o=$i << [format %c 4] >&@ stdout
	if {$i==0} {after 2000}
	after 500
}
