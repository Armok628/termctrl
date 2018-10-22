#!/usr/bin/wish
wm title . "Build Command Generator"
wm resizable . 0 0
ttk::labelframe .t -text "Build Type"
	ttk::radiobutton .t.game -text "Zone" -variable build -value "game"
	grid .t.game -row 0 -column 0 -sticky w -padx 5 -pady 5
	ttk::radiobutton .t.world -text "World" -variable build -value "world"
	grid .t.world -row 1 -column 0 -sticky w -padx 5 -pady 5
	set build "game"
grid .t -row 0 -column 0 -padx 5 -pady 5

ttk::labelframe .o -text "Extra Options"
	ttk::frame .o.d
		ttk::label .o.d.zl -text "Zone dimensions"
		ttk::entry .o.d.ze -textvariable zdims
			set zdims ""
		grid .o.d.zl -row 0 -column 0 -sticky w -padx 5 -pady 5
		grid .o.d.ze -row 0 -column 1 -sticky we -padx 5 -pady 5

		ttk::label .o.d.wl -text "World dimensions"
		ttk::entry .o.d.we -textvariable wdims
			set wdims ""
		grid .o.d.wl -row 1 -column 0 -sticky w -padx 5 -pady 5
		grid .o.d.we -row 1 -column 1 -sticky we -padx 5 -pady 5

		ttk::label .o.d.gl -text "Game dimensions"
		ttk::entry .o.d.ge -textvariable gdims
			set gdims ""
		grid .o.d.gl -row 2 -column 0 -sticky w -padx 5 -pady 5
		grid .o.d.ge -row 2 -column 1 -sticky we -padx 5 -pady 5
	grid .o.d -row 0 -column 0 -columnspan 2 -padx 5 -pady 5

	ttk::checkbutton .o.hemi -text "Hemisphere mode" -variable hemi -onvalue "-DHEMISPHERE" -offvalue ""
		set hemi "" 
	grid .o.hemi -row 1 -column 0 -padx 5 -pady 5 -sticky w

	ttk::checkbutton .o.scroll -text "Scrolling screen" -variable scroll -onvalue "-DSCROLL" -offvalue ""
		set scroll ""
	grid .o.scroll -row 1 -column 1 -padx 5 -pady 5 -sticky w

	ttk::checkbutton .o.no_weather -text "No weather" -variable no_weather -onvalue "-DNO_WEATHER" -offvalue ""
		set no_weather ""
	grid .o.no_weather -row 2 -column 0 -padx 5 -pady 5 -sticky w

	ttk::checkbutton .o.no_time -text "No day/night cycle" -variable no_time -onvalue "-DNO_TIME" -offvalue ""
		set no_time ""
	grid .o.no_time -row 2 -column 1 -padx 5 -pady 5 -sticky w
grid .o -row 1 -column 0 -padx 5 -pady 5 -sticky nswe

proc gencmd {} {
	upvar cflags cflags; upvar cmd cmd
	# Scan dimensional inputs
	lassign [regexp -all -inline {\d+} $::zdims] z_w z_h
	lassign [regexp -all -inline {\d+} $::wdims] w_w w_h
	lassign [regexp -all -inline {\d+} $::gdims] g_w g_h
	# Output command
	set cflags ""
	if {$z_w ne "" && $z_h ne ""} {append cflags " -DZ_WIDTH=$z_w -DZ_HEIGHT=$z_h"}
	if {$w_w ne "" && $w_h ne ""} {append cflags " -DW_WIDTH=$w_w -DW_HEIGHT=$w_h"}
	if {$g_w ne "" && $g_h ne ""} {append cflags " -DG_WIDTH=$g_w -DG_HEIGHT=$g_h"}
	if {$::hemi ne ""} {append cflags " $::hemi"}
	if {$::scroll ne ""} {append cflags " $::scroll"}
	if {$::no_weather ne ""} {append cflags " $::no_weather"}
	if {$::no_time ne ""} {append cflags " $::no_time"}
	set ::cmd "make $::build 'CFLAGS=$cflags'"
	.out select range 0 end
	focus .out
}
ttk::button .gen -text "Generate Command" -command gencmd
grid .gen -row 2 -column 0 -padx 5 -pady 5

ttk::entry .out -textvariable cmd
grid .out -row 3 -column 0 -padx 5 -pady 5 -sticky we
bind .out <<Copy>> {
	clipboard clear
	clipboard append $cmd
}

grid [ttk::button .comp -text "Compile" -command {
	gencmd
	update
	exec -- make $build CFLAGS=$cflags
	set sav [open ".lastcomp" w]
	foreach var [list build zdims wdims gdims hemi scroll no_weather no_time] {
		puts $sav "set $var {[set $var]}"
	}
	close $sav
}] -row 4 -column 0 -padx 5 -pady 5
if [file exists .lastcomp] {catch {source .lastcomp}}
focus .comp
