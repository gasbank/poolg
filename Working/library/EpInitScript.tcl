#set g_x 1985

proc EpInitGame {} {
	EpSetNextState 0
}

proc square_tcl {arg1} {
    set x [expr {$arg1 * $arg1}];
    return $x
}
proc printSquare {arg1} {
	set x [square_tcl $arg1];
	puts " printSquare result: $x"
}
# Beginning comment