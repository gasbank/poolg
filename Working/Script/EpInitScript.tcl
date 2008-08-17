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

####################################################################################

proc createHero { tileX tileY } {
	set curWorld [ EpGetCurWorld ];
	set unit [ EpCreateHero $tileX $tileY ];
	EpRegisterToWorld $curWorld $unit
	return $unit
}

proc createEnemy { tileX tileY { registerWorld 1 } } {
	set curWorld [ EpGetCurWorld ];
	set unit [ EpCreateEnemy $tileX $tileY ];
	if $registerWorld {
		EpRegisterToWorld $curWorld $unit
	}
	return $unit
}

####################################################################################

proc ToRadian {deg} {
	return [expr $deg * 3.14159265358979323846 / 180.0]
}

proc stacktrace {} {
	set stack "Stack trace:\n"
	for {set i 1} {$i < [info level]} {incr i} {
		set lvl [info level -$i]
		set pname [lindex $lvl 0]
		append stack [string repeat " " $i]$pname
		foreach value [lrange $lvl 1 end] arg [info args $pname] {
			if {$value eq ""} {
				info default $pname $arg value
			}
			append stack " $arg='$value'"
		}
		append stack \n
	}
	puts $stack
	return $stack
}
