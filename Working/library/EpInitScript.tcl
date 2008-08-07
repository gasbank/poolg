package require Thread

proc EpInitApp {} {
	# 16:9 widescreen resolution
	set w1 16
	set h1 9
	set multiplier 50
	EpSetWindowSize [expr $w1 * $multiplier] [expr $h1 * $multiplier]
}

proc EpInitGame {} {
	EpSetNextState 3
}

proc EpUnitOnMove {direction} {
	EpOutputDebugString "procedure called with argument: $direction\n"
}

proc square_tcl {arg1} {
	set x [expr {$arg1 * $arg1}];
	return $x
}

proc printSquare {arg1} {
	set x [square_tcl $arg1];
	puts " printSquare result: $x"
}

proc ToRadian {deg} {
	return [expr $deg * 3.14159265358979323846 / 180.0]
}
