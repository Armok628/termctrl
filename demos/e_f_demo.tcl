#!/usr/bin/tclsh
set seed [expr {int(rand()*1e10)}]
for {set i 5} {$i>=-5} {set i [expr {$i-0.5}]} {
	exec ./a.out seed=$seed {*}$argv e_f=$i >&@ stdout
	if {$i==1} {after 2000}
	after 500
}
