namespace eval EpWorldState {
	variable pWorld testEnemy pHeroUnit
	
	proc init {pCurWorld} {
		variable pWorld
		
		EpOutputDebugString " - WorldState init Ptr: $pCurWorld\n"
		
		set pWorld				$pCurWorld;
	}

	proc registerIncident1 {} {
		variable pHeroUnit
		variable testEnemy
		
		set trigger	 [ EpCreateUnitPositionTrigger $pHeroUnit 26 74 26 74 ]
		set action		[ EpCreateDialogAction "EpDialog1" ]
		set incident	[ EpCreateIncident $trigger $action ]
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncident2 {} {
		variable pHeroUnit
		variable testEnemy
			
		set trigger [ EpCreateUnitPositionTrigger $pHeroUnit 44 81 44 81 ]
		set action		[ EpCreateHealAction $pHeroUnit ]
		set incident	[ EpCreateIncident $trigger $action ]

		set action2		[ EpCreateDialogAction "EpDialogHeal" ]

		EpAddActionToIncident $incident $action2

		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncident3 {} {
		variable pWorld
		
		set trigger		[ EpCreateTotalAnnihilationTrigger $pWorld ]
		set action		[ EpCreateDialogAction "EpDialog2" ]
		set incident	[ EpCreateIncident $trigger $action ]

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncident4 {} {
		variable pHeroUnit
					
		set trigger [ EpCreateUnitPositionTrigger $pHeroUnit 26 98 26 98 ]
		set action		[ EpCreateUnitSpawnAction [createEnem 27 98 0] ]
		set incident	[ EpCreateIncident $trigger $action ]

		set action2		[ EpCreateUnitSpawnAction [createEnem 28 98 0] ]

		EpAddActionToIncident $incident $action2

		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncident5 {} {
		variable pWorld
		variable pHeroUnit
		
		set trigger		[ EpCreateUnTotalAnnihilationTrigger $pWorld ]
		set action		[ EpCreateDialogAction "EpDialogReturn" ]
		set incident		[ EpCreateIncident $trigger $action ]

		set trigger2		[ EpCreateUnitPositionTrigger $pHeroUnit 50 33 59 33 ]

		EpAddTriggerToIncident $incident $trigger2

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
	
	proc enter {} {
		variable testEnemy
		variable pHeroUnit
		
		EpOutputDebugString " - WorldState enter\n"
		
		set testEnemy				[createEnem 27 78 1];
		
		set pHeroUnit				[createHero 26 74];
		
		set pEnemyUnit1				[createEnem 25 90 1];
		set pEnemyUnit2				[createEnem 37 88 1];
		set pEnemyUnit3				[createEnem 35 67 1];
		set pEnemyUnit4				[createEnem 47 73 1];
		set pEnemyUnit5				[createEnem 62 83 1];
		set pEnemyUnit6				[createEnem 59 58 1];
		set pEnemyUnit7				[createEnem 36 53 1];
		set pEnemyUnit8				[createEnem 42 44 1];
		set pNPCUnit1				[createEnem 47 65 1];
		set pTestEnemyUnit			[createEnem 55 40 1];
		
		EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
                                                                  # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration		$pHeroUnit [expr 0.2]
		EpCharacterSetColor			$pHeroUnit 255 0 255
		
		
		EpCharacterSetColor			$pEnemyUnit1 128 128 0
		EpCharacterSetStat			$pEnemyUnit1 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit1 -1
		EpEnemySetRandomWalkable		$pEnemyUnit1 1
		EpCharacterSetBoundary			$pEnemyUnit1 23 92 27 88
		
		EpCharacterSetColor			$pEnemyUnit2 0 255 255
		EpCharacterSetStat			$pEnemyUnit2 3 1 4 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit2 -1
		EpEnemySetRandomWalkable		$pEnemyUnit2 1
		EpCharacterSetBoundary			$pEnemyUnit2 35 90 39 86

		
		EpCharacterSetColor			$pEnemyUnit3 255 255 0
		EpCharacterSetStat			$pEnemyUnit3 5 1 2 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit3 -1
		EpEnemySetRandomWalkable		$pEnemyUnit3 1
		EpCharacterSetBoundary			$pEnemyUnit3 33 69 37 65

		EpCharacterSetColor			$pEnemyUnit4 0 128 128
		EpCharacterSetStat			$pEnemyUnit4 2 1 5 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit4 -1
		EpEnemySetRandomWalkable		$pEnemyUnit4 1
		EpCharacterSetBoundary			$pEnemyUnit4 45 75 49 71

		EpCharacterSetColor			$pEnemyUnit5 128 0 128
		EpCharacterSetStat			$pEnemyUnit5 4 1 2 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit5 -1
		EpEnemySetRandomWalkable		$pEnemyUnit5 1
		EpCharacterSetBoundary			$pEnemyUnit5 60 85 64 81

		EpCharacterSetColor			$pEnemyUnit6 64 64 0
		EpCharacterSetStat			$pEnemyUnit6 3 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit6 -1
		EpEnemySetRandomWalkable		$pEnemyUnit6 1
		EpCharacterSetBoundary			$pEnemyUnit6 57 60 61 56

		EpCharacterSetColor			$pEnemyUnit7 0 64 64
		EpCharacterSetStat			$pEnemyUnit7 6 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit7 -1
		EpEnemySetRandomWalkable		$pEnemyUnit7 1
		EpCharacterSetBoundary			$pEnemyUnit7 34 55 38 51

		EpCharacterSetColor			$pEnemyUnit8 0 0 0
		EpCharacterSetStat			$pEnemyUnit8 2 1 5 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit8 -1
		EpEnemySetRandomWalkable		$pEnemyUnit8 1
		EpCharacterSetBoundary			$pEnemyUnit8 40 46 44 42
		
		EpCharacterSetColor			$pNPCUnit1 0 255 0
		
		# EpCharacterSetTalkable		$pNPCUnit1 1
		# EpCharacterSetRandomWalkable		$pNPCUnit1 1
		# EpCharacterSetBoundary		$pNPCUnit1 33 20 35 18  # left, top, right, bottom

		# Setting Test Enemy Unit
		EpCharacterSetColor			$pTestEnemyUnit 255 255 255
		EpEnemySetRandomWalkable		$pTestEnemyUnit	1
		EpCharacterSetBoundary			$pTestEnemyUnit 3 -22 5 -25  # left, top, right, bottom
		EpEnemySetTalkable			$pTestEnemyUnit	1
		
		
		registerIncident1
		registerIncident2
		registerIncident3
		registerIncident4
		registerIncident5
	}
	
	proc leave {} {
		EpOutputDebugString " - WorldState leave\n"
	}

	proc printDialogText {} {
		EpWriteDialog "ÇÑ±ÛÁ«¶óarlsdgklgtulsdu;asjd;tjhklsaerlushldfhlzxcvhxhjzvbkjzxcfjha;srhqwieutpuiwetp;oiweoptyiisauhljkdflkhzx"
	}

	# Not used procedure
	proc createChar { tileX tileY ctrl } {
		variable pWorld
		set unit [ EpCreateCharacter $tileX $tileY $ctrl ];
		EpRegisterToWorld $pWorld $unit
		return $unit
	}

	proc createHero { tileX tileY } {
		variable pWorld
		set unit [ EpCreateHero $tileX $tileY ];
		EpRegisterToWorld $pWorld $unit
		return $unit
	}

	proc createEnem { tileX tileY registerWorld } {
		variable pWorld
		set unit [ EpCreateEnemy $tileX $tileY ];
		if $registerWorld {
			EpRegisterToWorld $pWorld $unit
		}
		return $unit
	}	
}
