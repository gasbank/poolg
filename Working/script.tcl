#package require Thread

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
puts " --C proc-- The csum of 3 and 9 is: [csum 3 9]"
puts " --Tcl proc-- The square of 2 is: [square_tcl 2]"

puts "== OBJECT ALLOCATION =="
set widget [createWidget 5 7];
puts " Widget: a = [widgetGetA $widget] / b = [widgetGetB $widget] "

releaseWidget $widget



proc every {ms body} {eval $body; after $ms [info level 0]}
every 500 {puts "hello world~~~~~~~~~~~~~~~~~~"}


proc helloForever {} {
	
	thread::create {
		proc every {ms body} {eval $body; after $ms [info level 0]}


		every 500 {puts "xxxxx"}
		vwait forever
	}
	
}

proc helloForever2 {} {
	
	thread::create {
		proc every {ms body} {eval $body; after $ms [info level 0]}


		every 500 {puts "yyyy"}
		vwait forever
	}
	
}

proc listTest {} {
	puts "List Test Called.................................."
	return [list "kimgoyub is my name" ganada 1 2 3]
}

proc arrayTest {} {
	puts "Array Test Called.................................."
	set test(best) "This is the best!!!!!!! bestItem"
	set test(worst) "Sh*t! worstItem"
	set test(ok) "...justGoodItem... !"
	return [list $test(best) $test(worst) $test(ok)]
}