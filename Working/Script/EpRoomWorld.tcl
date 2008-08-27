source "Script/EpRoomWorld_EnterDoofuManQuest.tcl"

namespace eval EpRoomWorld {
	set modelFilePath	"RoomWorld.arn"
	set dialogNameList [ list  ]
	variable world
	variable pHeroUnit
	variable radiatorStartPos { 26 74 }
	variable doorFront { 50 42 }
	
	variable heroStartPos $radiatorStartPos
	
	proc init { curWorld } {
		variable world
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		
		set world				$curWorld;
	}

	proc registerIncidentHeal {} {
		variable pHeroUnit
		variable testEnemy
			
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 44 81 44 81 0x001 ]
		set action		[ EpCreateHealAction $pHeroUnit 500 ]
		set incident		[ EpCreateBlockingActionIncident $trigger $action 2 ]

		set action2		[ EpCreateDialogAction "EpDialogHeal" ]
		set action3		[ EpCreateTeleportAction $pHeroUnit 53 66 ]

		EpAddActionToIncident $incident $action2
		EpAddActionToIncident $incident $action3

		EpIncidentSetName	$incident "Heal incident"

		
		set incCount		[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentHidden {} {
		variable pHeroUnit
					
		set trigger [ EpCreateUnitPositionTrigger $pHeroUnit 26 98 26 98 0x001 ]
		set action		[ EpCreateUnitSpawnAction [createEnemy 27 98 0] ]
		set incident	[ EpCreateNonblockingActionIncident $trigger $action 1 ]

		set action2		[ EpCreateUnitSpawnAction [createEnemy 28 98 0] ]

		EpAddActionToIncident $incident $action2

		EpIncidentSetName	$incident "Hidden incident"

		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}	

	proc enter {} {
		variable testEnemy
		variable pHeroUnit
		variable pTestEnemyUnit
		variable TestStObject
		variable heroStartPos
		
		EpOutputDebugString " - [info level 0] called\n"
		
		set testEnemy				[createEnemy 46 75 1];
		
		set pHeroUnit				[getHero $heroStartPos];
		EpCharacterSetControllable	$pHeroUnit 1
		
		set pEnemyUnit1				[createEnemy 25 89 1];
		set pEnemyUnit2				[createEnemy 37 88 1];
		set pEnemyUnit3				[createEnemy 35 67 1];
		set pEnemyUnit4				[createEnemy 47 73 1];
		set pEnemyUnit5				[createEnemy 62 83 1];
		set pEnemyUnit6				[createEnemy 59 58 1];
		set pEnemyUnit7				[createEnemy 36 53 1];
		set pEnemyUnit8				[createEnemy 42 44 1];
		set pNPCUnit1				[createEnemy 47 65 1];
		set pTestEnemyUnit			[createEnemy 55 40 1];
		set testEnemy				[createEnemy 47 45 1];

		set TestStObject			[createStructureObject 50 40];
		EpUnitSetArnMesh			$TestStObject "PushableBox"

		
		
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]                                          
		EpCharacterSetStat			$pHeroUnit 100 5 7 5 5 5 ;# Health Will Coding Defence Sense Immunity
		EpCharacterSetCurHp			$pHeroUnit 100
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.15]
		EpUnitSetColor				$pHeroUnit 255 0 255
		
		
		EpUnitSetColor				$pEnemyUnit1 128 128 0
		EpCharacterSetStat			$pEnemyUnit1 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit1 -1
		EpEnemySetRandomWalkable	$pEnemyUnit1 1
		EpCharacterSetBoundary		$pEnemyUnit1 24 90 26 88
		
		EpUnitSetColor				$pEnemyUnit2 0 255 255
		EpCharacterSetStat			$pEnemyUnit2 3 1 4 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit2 -1
		EpEnemySetRandomWalkable	$pEnemyUnit2 1
		EpCharacterSetBoundary		$pEnemyUnit2 35 90 39 86

		
		EpUnitSetColor				$pEnemyUnit3 255 255 0
		EpCharacterSetStat			$pEnemyUnit3 5 1 2 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit3 -1
		EpEnemySetRandomWalkable	$pEnemyUnit3 1
		EpCharacterSetBoundary		$pEnemyUnit3 33 69 37 65

		EpUnitSetColor				$pEnemyUnit4 0 128 128
		EpCharacterSetStat			$pEnemyUnit4 2 1 5 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit4 -1
		EpEnemySetRandomWalkable	$pEnemyUnit4 1
		EpCharacterSetBoundary		$pEnemyUnit4 45 75 49 71

		EpUnitSetColor				$pEnemyUnit5 128 0 128
		EpCharacterSetStat			$pEnemyUnit5 4 1 2 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit5 -1
		EpEnemySetRandomWalkable	$pEnemyUnit5 1
		EpCharacterSetBoundary		$pEnemyUnit5 60 85 64 81

		EpUnitSetColor				$pEnemyUnit6 64 64 0
		EpCharacterSetStat			$pEnemyUnit6 3 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit6 -1
		EpEnemySetRandomWalkable	$pEnemyUnit6 1
		EpCharacterSetBoundary		$pEnemyUnit6 57 60 61 56

		EpUnitSetColor				$pEnemyUnit7 0 64 64
		EpCharacterSetStat			$pEnemyUnit7 6 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit7 -1
		EpEnemySetRandomWalkable	$pEnemyUnit7 1
		EpCharacterSetBoundary		$pEnemyUnit7 34 55 38 51

		EpUnitSetColor				$pEnemyUnit8 0 0 0
		EpCharacterSetStat			$pEnemyUnit8 2 1 5 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit8 -1
		EpEnemySetRandomWalkable	$pEnemyUnit8 1
		EpCharacterSetBoundary		$pEnemyUnit8 40 46 44 42
		
		EpUnitSetColor				$pNPCUnit1 0 255 0
		
		# Setting Test Enemy Unit
		EpUnitSetColor				$pTestEnemyUnit 255 255 255
		EpEnemySetRandomWalkable	$pTestEnemyUnit	0
		EpCharacterSetBoundary		$pTestEnemyUnit 3 -22 5 -25  ;# left, top, right, bottom
		EpEnemySetTalkable			$pTestEnemyUnit	1

		
		# Incidents ------------------------------------------------------------
		EpRegisterInitFadeInIncident		
		registerIncidentHeal
		registerIncidentHidden

		# Quests ---------------------------------------------------------------
		EnterDoofuManQuest::register
		
		# Warp Positions -------------------------------------------------------
		createWarpPosition			"EpA213World" GO 49 28 61 30
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

	proc printDialogText {} {
		EpWriteDialog "ÇÑ±ÛÁ«¶óarlsdgklgtulsdu;asjd;tjhklsaerlushldfhlzxcvhxhjzvbkjzxcfjha;srhqwieutpuiwetp;oiweoptyiisauhljkdflkhzx"
	}
}
