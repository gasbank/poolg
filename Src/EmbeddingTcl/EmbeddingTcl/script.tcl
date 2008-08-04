set g_x 1985

proc square_tcl {arg1} {
    set x [expr {$arg1 * $arg1}];
    return $x
}
proc printSquare {arg1} {
	set x [square_tcl $arg1];
	puts " printSquare result: $x"
}
# Beginning comment
puts "Hello, World - In quotes"    ;# This is a comment after the command.
puts " --C proc-- The square of 2 is: [square 2]"
puts " --Tcl proc-- The square of 2 is: [square_tcl 2]"