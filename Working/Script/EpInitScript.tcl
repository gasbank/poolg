#----------------------------------------------------------------------------------#
#                                                                                  #
#   EmptyProject: Initialization Script                                            #
#                                                                                  #
#----------------------------------------------------------------------------------#

variable EpStartTopStateName		PlayState
set EpStartWorldName				EpTestStage
set EpWorldList						[ list EpCeilingWorld EpRoomWorld EpA213World EpTestStage ]
set EpWindowMode					1

# This callback procedure is called when the D3D device is not created yet.
# Application level one-time initialization and constants are defined here.
proc EpInitApp {} {
	# Screen Resolution
	set w1 4
	set h1 3
	set multiplier 200
	EpSetWindowSize [expr $w1 * $multiplier] [expr $h1 * $multiplier]
}

# This callback procedure is called immediately after the creation of D3D device.
# It is not 'one-time', since D3D device can be recreated during the runtime.
# (By lost device event, for example.)
proc EpOnCreateDevice {} {
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
		set	pHeroUnit [ EpGetHero ]
	} else {
		# Hero is created only once
		set pHeroUnit [ EpCreateHero $tilePos ];
		EpCharacterSetMoveDuration	$pHeroUnit 0.05		
		EpCharacterSetStat			$pHeroUnit 7 5 7 5 5 5 ;# Health Will Coding Defence Sense Immunity
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpUnitSetColor				$pHeroUnit 255 0 255
		EpUnitSetArnMesh			$pHeroUnit "PoolGModel"
		EpRegisterToWorld			$curWorld $pHeroUnit
	}
	EpCharacterSetTilePos $pHeroUnit $tilePos
	return $pHeroUnit
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


proc createPosTrigTileRegion { unit tileRegion { flags 0x001 } } {
	return [ EpCreateUnitPositionTrigger $unit [lindex $tileRegion 0] [lindex $tileRegion 1] [lindex $tileRegion 2] [lindex $tileRegion 3] $flags ]
}

proc createPosTrigOneTile { unit tilePos { flags 0x001 } } {
	return [ EpCreateUnitPositionTrigger $unit [lindex $tilePos 0] [lindex $tilePos 1] -1 -1 $flags ]
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
