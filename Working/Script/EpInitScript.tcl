#----------------------------------------------------------------------------------#
#                                                                                  #
#   EmptyProject: Initialization Script                                            #
#                                                                                  #
#----------------------------------------------------------------------------------#

set EpWorldList [ list EpCeilingWorld EpRoomWorld EpA213World EpTestStage ]
set EpStartWorldName EpCeilingWorld

proc EpInitApp {} {
	# Screen Resolution
	set w1 4
	set h1 3
	set multiplier 200
	EpSetWindowSize [expr $w1 * $multiplier] [expr $h1 * $multiplier]
}

proc EpInitGame {} {
	
	# initialState:		1000	Intro
	#					1001	Play
	#					1002	Credit
	
	set initialState 1001
	EpSetNextState $initialState
	EpOutputDebugString "<Script> EpInitGame called\n"
}

#----------------------------------------------------------------------------------#

# Common Ep functions helpers

proc createHero { tilePos } {
	set curWorld [ EpGetCurWorld ];
	set hasHero EpHasHero
	if { $hasHero == 1 } {
		set	$unit EpGetHero
	} else {
		set unit [ EpCreateHero $tilePos ];
		EpRegisterToWorld $curWorld $unit
	}
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

# enum UnitPositionTriggerType {
# 	UPTT_ENTER  = 0x001,
#	UPTT_LEAVE  = 0x010,
#	UPTT_STAY	= 0x100 
# };

proc createWarpPosition { nextWorldName stopMoving tx0 ty0 { tx1 -1 } { ty1 -1 } } {
	set trigger			[ EpCreateUnitPositionTrigger [ EpGetHero ] $tx0 $ty0 $tx1 $ty1 0x001 ]
	set actions			[]
	if { $stopMoving eq "STOP" } {
		lappend actions		[ EpCreateControllableAction [ EpGetHero ] 0 ]
	}
	lappend actions		[ EpCreateScriptAction "EpChangeWorld $nextWorldName" ]
	set incident		[ EpCreateBlockingActionIncident $trigger 0 1 ]
	foreach act $actions {
		EpAddActionToIncident $incident $act
	}
	EpIncidentSetName $incident "Warp to $nextWorldName"
	
	set incCount	[ EpRegisterIncident $incident ]
	EpOutputDebugString " - Incident count: $incCount\n"
}

proc createStructureObject { tileX tileY { pushable 1 } } {
	set curWorld [ EpGetCurWorld ];
	set unit [ EpCreateStructureObject $tileX $tileY $pushable ];
	EpRegisterToWorld $curWorld $unit
	return $unit
}

#----------------------------------------------------------------------------------#

# Utilities

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
