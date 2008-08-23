namespace eval EpA213World {
	set modelFilePath	"A213World.arn"
	set dialogNameList [ list laserDialog laserDialog2 ]
	variable pHeroUnit
	variable gatePos { 14 66 }
	
	proc init { curWorld } {
		variable world
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		
		set world				$curWorld;
	}

	proc enter {} {
		variable pHeroUnit
		variable gatePos
		variable Box
		variable Mirror
		
		EpOutputDebugString " - [info level 0] called\n"
		
		set pHeroUnit				[ getHero $gatePos ];
		set pEnemyUnit1				[ createEnemy 81 70 ];
		set pEnemyUnit2				[ createEnemy 82 60 ];
		set pEnemyUnit3				[ createEnemy 64 64 ];
		set pEnemyUnit4				[ createEnemy 52 72 ];
		set pEnemyUnit5				[ createEnemy 73 85 ];
		set pEnemyUnit6				[ createEnemy 77 93 ];
		set pEnemyUnit7				[ createEnemy 71 89 ];
		set pEnemyUnit8				[ createEnemy 64 97 ];
		set pEnemyUnit9				[ createEnemy 45 109 ];
		set pEnemyUnit10			[ createEnemy 81 103 ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetControllable	$pHeroUnit 1
		#EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		#EpCharacterSetCurHp			$pHeroUnit 10
		#EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpUnitSetColor			$pHeroUnit 255 0 255
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]

		EpUnitSetColor					$pEnemyUnit1 255 255 255
		EpCharacterSetStat				$pEnemyUnit1 4 1 3 1 1 1
		EpCharacterSetCurHp				$pEnemyUnit1 -1
		EpEnemySetRandomWalkable		$pEnemyUnit1 1
		EpCharacterSetBoundary			$pEnemyUnit1 78 67 84 73
		EpUnitSetArnMesh				$pEnemyUnit1 "DinoModel"

		EpUnitSetColor					$pEnemyUnit2 255 255 0
		EpCharacterSetStat				$pEnemyUnit2 4 1 3 1 1 1
		EpCharacterSetCurHp				$pEnemyUnit2 -1
		EpEnemySetRandomWalkable		$pEnemyUnit2 1
		EpCharacterSetBoundary			$pEnemyUnit2 79 57 85 63
		EpUnitSetArnMesh				$pEnemyUnit2 "DinoModel"

		EpUnitSetColor				$pEnemyUnit3 255 0 255
		EpCharacterSetStat			$pEnemyUnit3 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit3 -1
		EpEnemySetRandomWalkable		$pEnemyUnit3 1
		EpCharacterSetBoundary			$pEnemyUnit3 61 61 67 67
		EpUnitSetArnMesh				$pEnemyUnit3 "DinoModel"

		EpUnitSetColor				$pEnemyUnit4 0 255 255
		EpCharacterSetStat			$pEnemyUnit4 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit4 -1
		EpEnemySetRandomWalkable		$pEnemyUnit4 1
		EpCharacterSetBoundary			$pEnemyUnit4 49 69 55 75
		EpUnitSetArnMesh				$pEnemyUnit4 "DinoModel"

		EpUnitSetColor				$pEnemyUnit5 0 0 255
		EpCharacterSetStat			$pEnemyUnit5 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit5 -1
		EpEnemySetRandomWalkable		$pEnemyUnit5 1
		EpCharacterSetBoundary			$pEnemyUnit5 70 82 76 89
		EpUnitSetArnMesh				$pEnemyUnit5 "DinoModel"

		EpUnitSetColor				$pEnemyUnit6 0 255 0
		EpCharacterSetStat			$pEnemyUnit6 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit6 -1
		EpEnemySetRandomWalkable		$pEnemyUnit6 1
		EpCharacterSetBoundary			$pEnemyUnit6 74 90 80 96
		EpUnitSetArnMesh				$pEnemyUnit6 "DinoModel"

		EpUnitSetColor				$pEnemyUnit7 255 0 0
		EpCharacterSetStat			$pEnemyUnit7 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit7 -1
		EpEnemySetRandomWalkable		$pEnemyUnit7 1
		EpCharacterSetBoundary			$pEnemyUnit7 68 86 74 92
		EpUnitSetArnMesh				$pEnemyUnit7 "DinoModel"

		EpUnitSetColor				$pEnemyUnit8 128 128 128
		EpCharacterSetStat			$pEnemyUnit8 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit8 -1
		EpEnemySetRandomWalkable		$pEnemyUnit8 1
		EpCharacterSetBoundary			$pEnemyUnit8 61 94 67 100
		EpUnitSetArnMesh				$pEnemyUnit8 "DinoModel"

		EpUnitSetColor				$pEnemyUnit9 128 0 0
		EpCharacterSetStat			$pEnemyUnit9 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit9 -1
		EpEnemySetRandomWalkable		$pEnemyUnit9 1
		EpCharacterSetBoundary			$pEnemyUnit9 42 106 48 112
		EpUnitSetArnMesh				$pEnemyUnit9 "DinoModel"

		EpUnitSetColor				$pEnemyUnit10 0 0 0
		EpCharacterSetStat			$pEnemyUnit10 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit10 -1
		EpEnemySetRandomWalkable		$pEnemyUnit10 1
		EpCharacterSetBoundary			$pEnemyUnit10 78 100 84 106
		EpUnitSetArnMesh				$pEnemyUnit10 "DinoModel"

		set Box					[createStructureObject 6 70];
		EpUnitSetArnMesh			$Box "PushableBox"

		set Mirror				[createStructureObject 6 60];
		EpUnitSetArnMesh			$Mirror "PushableBox"
		
		Incident_GateOpen
		Incident_BossGateOpen
		registerIncidentBox
		registerIncidentMirror
		registerIncidentOpen
		registerIncidentOpen2
		EpRegisterInitFadeInIncident
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
		lappend actions		[ EpCreateDelayAction			10000 ]
		lappend actions		[ EpCreateCameraAction			attach GateCamera 1500 ]
		lappend actions		[ EpCreateControllableAction	$pHeroUnit 1 ]
		set incident		[ EpCreateBlockingActionIncident $trigger 0 1 ]
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
	
	proc Incident_BossGateOpen {} {
		variable pHeroUnit
		set animObjects [ list Blocking1 Blocking2 Blocking3 GateRight GateLeft GateCamera ]
		
		set trigger			[ EpCreateUnitPositionTrigger	$pHeroUnit 42 57 42 71 0x001 ]
		set actions			[ list ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode LaserConeDeco  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Laser1  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Laser2  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode BossWall  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode BossWallEventCamera ] 1" ]
		lappend actions		[ EpCreateCameraAction			external BossWallEventCamera 0 ]
		lappend actions		[ EpCreateDelayAction			17000 ]
		lappend actions		[ EpCreateCameraAction			attach BossWallEventCamera 1500 ]
		set incident		[ EpCreateBlockingActionIncident $trigger 0 1 ]
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentBox {} {
		variable world
		variable Box
		variable Mirror
		
		set triggers		[ EpCreateUnitPositionTrigger $Box 9 70 -1 -1 0x001 ]
		set trigger	[ EpCreateUnitPositionTrigger $Mirror 9 60 -1 -1 0x100 ]
		lappend triggers	[ EpCreateReverseTrigger $trigger ]
		set actions		[ EpCreateFadeAction out 3500 ]
		lappend actions		[ EpCreateFadeAction in 3000 ]
		set incident	[ EpCreateBlockingActionIncident 0 0 -1 ]

		foreach trig $triggers {
			EpAddTriggerToIncident $incident $trig
		}

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Box incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentMirror {} {
		variable world
		variable Box
		variable Mirror
		
		set trigger		[ EpCreateUnitPositionTrigger $Mirror 9 60 -1 -1 0x001 ]
		set actions		[ EpCreateFadeAction out 3500 ]
		lappend actions		[ EpCreateFadeAction in 3000 ]
		set incident	[ EpCreateBlockingActionIncident $trigger 0 -1 ]

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Mirror incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentOpen {} {
		variable world
		variable Box
		variable Mirror

		set triggers		[ EpCreateUnitPositionTrigger $Box 9 70 -1 -1 0x100 ]
		lappend triggers		[ EpCreateUnitPositionTrigger $Mirror 9 60 -1 -1 0x001 ]
		set actions		[ EpCreateDialogAction "EpA213World::laserDialog" ]
		set incident	[ EpCreateBlockingActionIncident 0 0 1 ]

		foreach trig $triggers {
			EpAddTriggerToIncident $incident $trig
		}

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Open incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentOpen2 {} {
		variable world
		variable Box
		variable Mirror

		set triggers		[ EpCreateUnitPositionTrigger $Box 9 70 -1 -1 0x001 ]
		lappend triggers		[ EpCreateUnitPositionTrigger $Mirror 9 60 -1 -1 0x100 ]
		set actions		[ EpCreateDialogAction "EpA213World::laserDialog2" ]
		set incident	[ EpCreateBlockingActionIncident 0 0 1 ]

		foreach trig $triggers {
			EpAddTriggerToIncident $incident $trig
		}

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Open2 incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
		
	proc Incident_GateOpen_NewMethod {} {
			variable pHeroUnit
			set animObjectNames [ list Blocking1 Blocking2 Blocking3 GateRight GateLeft GateCamera ]
			set animObjects [ list ]
			foreach objName $animObjectNames {
				lappend xformables [ EpGetNode $objName ]
			}
			
			set trigger			[ EpCreateUnitPositionTrigger	$pHeroUnit 23 75 21 57 0x001 ]
			set actions			[ EpCreateControllableAction	$pHeroUnit 0 ]
			lappend actions		[ EpCreateCameraAction			external GateCamera 0 ]
			lappend actions		[ EpCreateStartAnimationAction	$animObjects ]
			lappend actions		[ EpCreateCameraAction			attach GateCamera 1500 ]
			lappend actions		[ EpCreateControllableAction	$pHeroUnit 1 ]
			
			set incident		[ EpCreateBlockingActionIncident $trigger 0 1 ]
			foreach act $actions {
				EpAddActionToIncident $incident $act
			}
	
			set incCount	[ EpRegisterIncident $incident ]
			EpOutputDebugString " - Incident count: $incCount\n"
		}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

	namespace eval laserDialog {	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "SYSTEM"
	
		set dialog [ list\
			$player		"레이져가 통과하다가 꺾였다?"\
		];
	}

	namespace eval laserDialog2 {	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "SYSTEM"
	
		set dialog [ list\
			$player		"레이져가 시작하다가 꺾였다?"\
		];
	}
}
