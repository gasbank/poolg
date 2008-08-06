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
