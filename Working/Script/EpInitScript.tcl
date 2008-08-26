#----------------------------------------------------------------------------------#
#                                                                                  #
#   EmptyProject: Initialization Script                                            #
#                                                                                  #
#----------------------------------------------------------------------------------#

variable EpStartTopStateName		PlayState
set EpStartWorldName				EpTestStage
set EpWorldList						[ list EpCeilingWorld EpRoomWorld EpA213World EpTestStage ]

proc EpInitApp {} {
	# Screen Resolution
	set w1 4
	set h1 3
	set multiplier 200
	EpSetWindowSize [expr $w1 * $multiplier] [expr $h1 * $multiplier]
}

proc EpInitGame {} {
	variable EpStartTopStateName
	
	set TOP_STATE(IntroState)		1000
	set TOP_STATE(PlayState)		1001
	set TOP_STATE(CreditState)		1002
		
	EpSetNextState $TOP_STATE($EpStartTopStateName)
	EpOutputDebugString "<Script> EpInitGame called\n"
}

#----------------------------------------------------------------------------------#

# Common Ep functions helpers

proc getHero { tilePos } {
	set curWorld [ EpGetCurWorld ];
	set hasHero [ EpHasHero ];
	if { $hasHero == 1 } {
		set	unit [ EpGetHero ]
	} else {
		# Hero is created only once
		set unit [ EpCreateHero $tilePos ];
		EpCharacterSetMoveDuration	$unit	0.05
		EpRegisterToWorld $curWorld $unit
	}
	EpCharacterSetTilePos $unit $tilePos
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
proc createEnemy2 { tilePos { registerWorld 1 } } {
	return [ createEnemy [lindex $tilePos 0] [lindex $tilePos 1] $registerWorld ]
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
	lappend actions		[ EpCreateFadeAction "out" 1500 ]
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
proc createStructureObject2 { tilePos { pushable 1 } } {
	return [ createStructureObject [lindex $tilePos 0] [lindex $tilePos 1] $pushable ]
}

proc EpRegisterInitFadeInIncident {} {
	set trigger		[ EpCreateNullTrigger ]
	set action		[ EpCreateFadeAction "in" 250 ]
	set incident	[ EpCreateNonblockingActionIncident $trigger $action 1 ]

	EpIncidentSetName	$incident "init fade incident"

	set incCount	[ EpRegisterIncident	$incident ]
	EpOutputDebugString " - Incident count: $incCount\n"
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
