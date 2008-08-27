source "Script/EpRoomWorld_EnterDoofuManQuest.tcl"

namespace eval EpRoomWorld {
	set modelFilePath	"RoomWorld.arn"
	set dialogNameList [ list ]
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
			
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 44 81 44 81 0x001 ]
		set action		[ EpCreateHealAction $pHeroUnit 500 ]
		set incident	[ EpCreateBlockingActionIncident $trigger $action 2 ]

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
					
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 26 98 26 98 0x001 ]
		set action		[ EpCreateUnitSpawnAction [createEnemy 27 98 0] ]
		set incident	[ EpCreateNonblockingActionIncident $trigger $action 1 ]

		set action2		[ EpCreateUnitSpawnAction [createEnemy 28 98 0] ]

		EpAddActionToIncident $incident $action2

		EpIncidentSetName	$incident "Hidden incident"

		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}	

	proc enter {} {
		variable pHeroUnit
		variable heroStartPos
		
		EpOutputDebugString " - [info level 0] called\n"
				
		set pHeroUnit				[getHero $heroStartPos];
		EpCharacterSetControllable	$pHeroUnit 1

		createEnemyGwengYi { 25 89 }
		createEnemyGwengYi { 37 88 }
		createEnemyGwengYi { 35 67 }
		createEnemyGwengYi { 47 73 }
		createEnemyGwengYi { 62 83 }
		createEnemyGwengYi { 59 58 }
		createEnemyGwengYi { 36 53 }
		createEnemyGwengYi { 42 44 }
		createEnemyGwengYi { 47 65 }
	
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

	proc createEnemyGwengYi { tilePos } {
		set gwengYi [ createEnemy2 $tilePos ];

		EpUnitSetColor				$gwengYi 128 128 0
		EpCharacterSetStat			$gwengYi 4 1 3 1 1 1
		EpCharacterSetCurHp			$gwengYi -1
		EpEnemySetRandomWalkable	$gwengYi 1
		EpCharacterSetAutoBoundary	$gwengYi 1
	}
}
