set EpWorldList [ list EpCeilingWorld EpRoomWorld EpA213World ]

proc EpInitApp {} {
	# 16:9 widescreen resolution
	set w1 4
	set h1 3
	set multiplier 200
	EpSetWindowSize [expr $w1 * $multiplier] [expr $h1 * $multiplier]
	
}

proc EpInitGame {} {
	set initialState 1001
	EpSetNextState $initialState
	EpOutputDebugString "<Script> EpInitGame called\n"
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
