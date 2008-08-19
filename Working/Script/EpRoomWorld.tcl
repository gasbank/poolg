namespace eval EpRoomWorld {
	set modelFilePath	"RoomWorld.arn"
	set dialogNameList [ list  ]
	variable world testEnemy pHeroUnit pTestEnemyUnit
	
	proc init { curWorld } {
		variable world
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		
		set world				$curWorld;
	}

	proc registerIncidentHeal {} {
		variable pHeroUnit
		variable testEnemy
			
		set trigger [ EpCreateUnitPositionTrigger $pHeroUnit 44 81 44 81 0x001 ]
		set action		[ EpCreateHealAction $pHeroUnit ]
		set incident	[ EpCreateBlockingActionIncident $trigger $action 1 ]

		set action2		[ EpCreateDialogAction "EpDialogHeal" ]
		set action3		[ EpCreateTeleportAction $pHeroUnit 53 45 ]

		EpAddActionToIncident $incident $action2
		EpAddActionToIncident $incident $action3

		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentClear {} {
		variable pHeroUnit
		variable world
		
		set trigger		[ EpCreateTotalAnnihilationTrigger $world ]
		set actions		[ EpCreateFadeAction out 3500 ]
		set incident	[ EpCreateBlockingActionIncident $trigger 0 0 ]

		lappend actions		[ EpCreateTeleportAction $pHeroUnit 53 45 ]
		lappend actions		[ EpCreateFadeAction in 3000 ]
		lappend actions		[ EpCreateScriptAction "EpSetDoAnim [EpGetNode Door] 1" ]
		lappend actions		[ EpCreateScriptAction "EpSetDoAnim [EpGetNode TofuMan] 1" ]
		lappend actions		[ EpCreateDialogAction "EpDialog2" ]
				
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
	
	proc registerIncidentHidden {} {
		variable pHeroUnit
					
		set trigger [ EpCreateUnitPositionTrigger $pHeroUnit 26 98 26 98 0x001 ]
		set action		[ EpCreateUnitSpawnAction [createEnemy 27 98 0] ]
		set incident	[ EpCreateIncident $trigger $action 0 ]

		set action2		[ EpCreateUnitSpawnAction [createEnemy 28 98 0] ]

		EpAddActionToIncident $incident $action2

		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentUnClear {} {
		variable world
		variable pHeroUnit
		variable pTestEnemyUnit
		
		set trigger		[ EpCreateUnTotalAnnihilationTrigger $world ]
		set action		[ EpCreateDialogAction "EpDialogReturn" ]
		set incident		[ EpCreateIncident $trigger $action 1 ]

		set trigger2		[ EpCreateUnitPositionTrigger $pHeroUnit 50 33 59 33 0x001 ]
		set action2		[ EpCreateUnitMoveAction $pHeroUnit "UP" ]
		set action3		[ EpCreateUnitMoveAction $pTestEnemyUnit "DOWN" ]

		EpAddTriggerToIncident $incident $trigger2
		EpAddActionToIncident $incident $action2
		EpAddActionToIncident $incident $action3

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
	
	proc Incident_TofuManEnter {} {
		variable pHeroUnit
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 50 42 59 42 0x001 ]
		set action0		[ EpCreateScriptAction "EpSetDoAnim [EpGetNode Door] 1" ]
		set action1		[ EpCreateScriptAction "EpSetDoAnim [EpGetNode TofuMan] 1" ]
		
		set incident	[ EpCreateIncident $trigger $action0 0 ]
		EpAddActionToIncident $incident $action1
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentSwitch {} {
		variable world
		variable TestStObject
		variable pTestEnemyUnit
		
		set trigger		[ EpCreateUnitPositionTrigger $TestStObject 50 35 -1 -1 0x101 ]
		set action		[ EpCreateScriptAction "EpSetDoAnim [EpGetNode Door] 1" ]
		set incident		[ EpCreateIncident $trigger $action 1 ]

		set action2		[ EpCreateScriptAction "EpSetDoAnim [EpGetNode TofuMan] 1" ]
		
		EpAddActionToIncident $incident $action2
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentCamera {} {
		variable pHeroUnit
		
		set trigger		[ EpCreateUnitPositionTrigger	$pHeroUnit 52 45 52 45 0x101 ]
		set action		[ EpCreateCameraAction			external Camera 0 ]
		set incident	[ EpCreateIncident				$trigger $action 0 ]

		set incCount	[ EpRegisterIncident			$incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentCamera2 {} {
		variable pHeroUnit
		
		set trigger		[ EpCreateUnitPositionTrigger	$pHeroUnit 54 45 54 45 0x101 ]
		set action		[ EpCreateCameraAction			attach 0 5000 ]
		set incident	[ EpCreateIncident				$trigger $action 0 ]

		set incCount	[ EpRegisterIncident			$incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
	
	proc registerIncidentControl {} {
		variable pHeroUnit

		set trigger		[ EpCreateUnitPositionTrigger	$pHeroUnit 55 45 55 45 0x101 ]
		set action		[ EpCreateControllableAction	$pHeroUnit 1 ]
		set incident	[ EpCreateIncident				$trigger $action 0 ]

		set incCount	[ EpRegisterIncident			$incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc enter {} {
		variable testEnemy
		variable pHeroUnit
		variable pTestEnemyUnit
		variable TestStObject
		
		EpOutputDebugString " - [info level 0] called\n"
		
		set testEnemy				[createEnemy 27 78 1];
		
		set pHeroUnit				[createHero 53 45];
		
		set pEnemyUnit1				[createEnemy 25 90 1];
		set pEnemyUnit2				[createEnemy 37 88 1];
		set pEnemyUnit3				[createEnemy 35 67 1];
		set pEnemyUnit4				[createEnemy 47 73 1];
		set pEnemyUnit5				[createEnemy 62 83 1];
		set pEnemyUnit6				[createEnemy 59 58 1];
		set pEnemyUnit7				[createEnemy 36 53 1];
		set pEnemyUnit8				[createEnemy 42 44 1];
		set pNPCUnit1				[createEnemy 47 65 1];
		set pTestEnemyUnit			[createEnemy 55 40 1];

		set TestStObject			[createStructureObject 50 40];
		EpUnitSetArnMesh			$TestStObject "PushableBox"
		
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
                                             # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    100     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 100
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration		$pHeroUnit [expr 0.05]
		EpUnitSetColor			$pHeroUnit 255 0 255
		
		
		EpUnitSetColor			$pEnemyUnit1 128 128 0
		EpCharacterSetStat			$pEnemyUnit1 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit1 -1
		EpEnemySetRandomWalkable		$pEnemyUnit1 1
		EpCharacterSetBoundary			$pEnemyUnit1 23 92 27 88
		
		EpUnitSetColor			$pEnemyUnit2 0 255 255
		EpCharacterSetStat			$pEnemyUnit2 3 1 4 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit2 -1
		EpEnemySetRandomWalkable		$pEnemyUnit2 1
		EpCharacterSetBoundary			$pEnemyUnit2 35 90 39 86

		
		EpUnitSetColor			$pEnemyUnit3 255 255 0
		EpCharacterSetStat			$pEnemyUnit3 5 1 2 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit3 -1
		EpEnemySetRandomWalkable		$pEnemyUnit3 1
		EpCharacterSetBoundary			$pEnemyUnit3 33 69 37 65

		EpUnitSetColor			$pEnemyUnit4 0 128 128
		EpCharacterSetStat			$pEnemyUnit4 2 1 5 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit4 -1
		EpEnemySetRandomWalkable		$pEnemyUnit4 1
		EpCharacterSetBoundary			$pEnemyUnit4 45 75 49 71

		EpUnitSetColor			$pEnemyUnit5 128 0 128
		EpCharacterSetStat			$pEnemyUnit5 4 1 2 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit5 -1
		EpEnemySetRandomWalkable		$pEnemyUnit5 1
		EpCharacterSetBoundary			$pEnemyUnit5 60 85 64 81

		EpUnitSetColor			$pEnemyUnit6 64 64 0
		EpCharacterSetStat			$pEnemyUnit6 3 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit6 -1
		EpEnemySetRandomWalkable		$pEnemyUnit6 1
		EpCharacterSetBoundary			$pEnemyUnit6 57 60 61 56

		EpUnitSetColor			$pEnemyUnit7 0 64 64
		EpCharacterSetStat			$pEnemyUnit7 6 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit7 -1
		EpEnemySetRandomWalkable		$pEnemyUnit7 1
		EpCharacterSetBoundary			$pEnemyUnit7 34 55 38 51

		EpUnitSetColor			$pEnemyUnit8 0 0 0
		EpCharacterSetStat			$pEnemyUnit8 2 1 5 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit8 -1
		EpEnemySetRandomWalkable		$pEnemyUnit8 1
		EpCharacterSetBoundary			$pEnemyUnit8 40 46 44 42
		
		EpUnitSetColor			$pNPCUnit1 0 255 0
		
		# EpCharacterSetTalkable		$pNPCUnit1 1
		# EpCharacterSetRandomWalkable		$pNPCUnit1 1
		# EpCharacterSetBoundary		$pNPCUnit1 33 20 35 18  # left, top, right, bottom

		# Setting Test Enemy Unit
		EpUnitSetColor			$pTestEnemyUnit 255 255 255
		EpEnemySetRandomWalkable		$pTestEnemyUnit	0
		EpCharacterSetBoundary			$pTestEnemyUnit 3 -22 5 -25  ;# left, top, right, bottom
		EpEnemySetTalkable			$pTestEnemyUnit	1

		
		# Incidents ------------------------------------------------------------
		registerIncidentHeal
		registerIncidentClear
		registerIncidentHidden
		#registerIncidentUnClear
		#Incident_TofuManEnter
		registerIncidentSwitch
		registerIncidentCamera
		registerIncidentCamera2
		registerIncidentControl
		
		# Warp Positions -------------------------------------------------------
		#createWarpPosition			"EpA213World" 49 28 61 30
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

	proc printDialogText {} {
		EpWriteDialog "ÇÑ±ÛÁ«¶óarlsdgklgtulsdu;asjd;tjhklsaerlushldfhlzxcvhxhjzvbkjzxcfjha;srhqwieutpuiwetp;oiweoptyiisauhljkdflkhzx"
	}
}
