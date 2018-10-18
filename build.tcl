#!/usr/bin/wish
ttk::labelframe .t -text "Build type"
	ttk::radiobutton .t.game -text "Zone" -variable build -value ""
	grid .t.game -row 0 -column 0 -sticky w -padx 5 -pady 5

	ttk::radiobutton .t.world -text "World" -variable build -value " world"
		set build ""
	grid .t.world -row 1 -column 0 -sticky w -padx 5 -pady 5
grid .t -row 0 -column 0 -padx 5 -pady 5

ttk::labelframe .o -text "Extra options"
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
	grid .o.hemi -row 1 -column 0 -padx 5 -pady 5

	ttk::checkbutton .o.scroll -text "Scrolling screen" -variable scroll -onvalue "-DSCROLL" -offvalue ""
		set scroll ""
	grid .o.scroll -row 1 -column 1 -padx 5 -pady 5
grid .o -row 1 -column 0 -padx 5 -pady 5

ttk::button .gen -text "Generate Command" -command {
	# Scan dimensional inputs
	lassign [regexp -all -inline {\d+} $zdims] z_w z_h
	lassign [regexp -all -inline {\d+} $wdims] w_w w_h
	lassign [regexp -all -inline {\d+} $gdims] g_w g_h
	# Print command
	puts -nonewline "make$build 'CFLAGS="
	if {$z_w ne "" && $z_h ne ""} {puts -nonewline " -DZ_WIDTH=$z_w -DZ_HEIGHT=$z_h"}
	if {$w_w ne "" && $w_h ne ""} {puts -nonewline " -DW_WIDTH=$w_w -DW_HEIGHT=$w_h"}
	if {$g_w ne "" && $g_h ne ""} {puts -nonewline " -DG_WIDTH=$g_w -DG_HEIGHT=$g_h"}
	if {$hemi ne ""} {puts -nonewline " $hemi"}
	if {$scroll ne ""} {puts -nonewline " $scroll"}
	puts "'"
	exit 0
}
grid .gen -row 2 -column 0 -padx 5 -pady 5
