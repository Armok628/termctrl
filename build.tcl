#!/usr/bin/wish
wm title . "Build Command Generator"
wm resizable . 0 0
ttk::labelframe .t -text "Build Type"
	ttk::radiobutton .t.world -text "World" -variable build -value "world"
	grid .t.world -row 0 -column 0 -sticky w -padx 5 -pady 5
	ttk::radiobutton .t.game -text "Zone" -variable build -value "zone"
	grid .t.game -row 1 -column 0 -sticky w -padx 5 -pady 5
	set build "world"
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

		ttk::label .o.d.fl -text "Number of factions"
		ttk::entry .o.d.fe -textvariable factions
			set factions 7
		grid .o.d.fl -row 3 -column 0 -sticky w -padx 5 -pady 5
		grid .o.d.fe -row 3 -column 1 -sticky we -padx 5 -pady 5
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

ttk::labelframe .d -text "Default World Generation Options"
	ttk::label .d.at -text "Age"
	ttk::entry .d.ae -textvariable age
		set age 4
	grid .d.at -row 0 -column 0 -sticky we -padx 5 -pady 5
	grid .d.ae -row 0 -column 1 -sticky we -padx 5 -pady 5

	ttk::label .d.eft -text "Elevation Factor"
	ttk::entry .d.efe -textvariable e_f
		set e_f "1.0"
	grid .d.eft -row 1 -column 0 -sticky we -padx 5 -pady 5
	grid .d.efe -row 1 -column 1 -sticky we -padx 5 -pady 5

	ttk::label .d.tft -text "Temperature Factor"
	ttk::entry .d.tfe -textvariable t_f
		set t_f "1.0"
	grid .d.tft -row 2 -column 0 -sticky we -padx 5 -pady 5
	grid .d.tfe -row 2 -column 1 -sticky we -padx 5 -pady 5

	ttk::label .d.eot -text "Elevation Offset"
	ttk::entry .d.eoe -textvariable e_o
		set e_o "0"
	grid .d.eot -row 3 -column 0 -sticky we -padx 5 -pady 5
	grid .d.eoe -row 3 -column 1 -sticky we -padx 5 -pady 5

	ttk::label .d.tot -text "Temperature Offset"
	ttk::entry .d.toe -textvariable t_o
		set t_o "0"
	grid .d.tot -row 4 -column 0 -sticky we -padx 5 -pady 5
	grid .d.toe -row 4 -column 1 -sticky we -padx 5 -pady 5
grid .d -row 2 -column 0 -padx 5 -pady 5 -sticky nswe

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
	if {$::age ne "4"} {append cflags " -DDEF_AGE=$::age"}
	if {$::e_f ne "1.0"} {append cflags " -DDEF_E_F=$::e_f"}
	if {$::t_f ne "1.0"} {append cflags " -DDEF_T_F=$::t_f"}
	if {$::e_o ne "0"} {append cflags " -DDEF_E_O=$::e_o"}
	if {$::t_o ne "0"} {append cflags " -DDEF_T_O=$::t_o"}
	if {$::factions ne "7"} {append cflags " -DMAX_FACTIONS=$::factions"}
	set ::cmd "make $::build 'CFLAGS=$cflags'"
	.out select range 0 end
	focus .out
}
ttk::button .gen -text "Generate Command" -command gencmd
grid .gen -row 3 -column 0 -padx 5 -pady 5

ttk::entry .out -textvariable cmd
grid .out -row 4 -column 0 -padx 5 -pady 5 -sticky we
bind .out <<Copy>> {
	clipboard clear
	clipboard append $cmd
}

grid [ttk::button .comp -text "Compile" -command {
	gencmd
	update
	exec -- make $build CFLAGS=$cflags
	set sav [open ".lastcomp" w]
	foreach var [list build zdims wdims gdims hemi scroll no_weather no_time age e_f t_f e_o t_o factions] {
		puts $sav "set $var {[set $var]}"
	}
	close $sav
}] -row 5 -column 0 -padx 5 -pady 5
if [file exists .lastcomp] {catch {source .lastcomp}}
focus .comp
