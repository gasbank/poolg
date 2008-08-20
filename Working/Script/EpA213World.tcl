namespace eval EpA213World {
	set modelFilePath	"A213World.arn"
	set dialogNameList [ list  ]
	variable pHeroUnit
	
	proc init { curWorld } {
		variable world
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		
		set world				$curWorld;
	}

	proc enter {} {
		variable pHeroUnit
		
		EpOutputDebugString " - [info level 0] called\n"
		
		set pHeroUnit				[ createHero 14 66 ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpUnitSetColor			$pHeroUnit 255 0 255
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		
		Incident_GateOpen
	}
	
	
	proc Incident_GateOpen {} {
		variable pHeroUnit
		set animObjects [ list Blocking1 Blocking2 Blocking3 GateRight GateLeft GateCamera ]
		
		set trigger			[ EpCreateUnitPositionTrigger	$pHeroUnit 23 75 21 57 0x001 ]
		set actions			[ EpCreateControllableAction	$pHeroUnit 0 ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Blocking1  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Blocking2  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Blocking3  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode GateRight  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode GateLeft   ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode GateCamera ] 1" ]
		lappend actions		[ EpCreateCameraAction			external GateCamera 0 ]
		lappend actions		[ EpCreateDelayAction			15000 ]
		lappend actions		[ EpCreateCameraAction			attach GateCamera 1500 ]
		lappend actions		[ EpCreateControllableAction	$pHeroUnit 1 ]
		set incident		[ EpCreateBlockingActionIncident $trigger 0 0 ]
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

}
