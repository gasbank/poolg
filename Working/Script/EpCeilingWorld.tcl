namespace eval EpCeilingWorld {
	set modelFilePath	"ceiling.arn"
	global world hero npcGetg npcGloop pHeroUnit

	proc registerIncidentInitTalk {} {
		global world hero npcGetg npcGloop pHeroUnit
		
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 30 77 30 77 ]
		set actions		[ EpCreateDialogAction "EpDialog1" ]
		lappend actions	[ EpCreateDialogAction "EpDialog2" ]
		lappend actions	[ EpCreateFadeAction out 3500 ]
		lappend actions	[ EpCreateDialogAction "EpDialog3" ]
		lappend actions	[ EpCreateFadeAction in 3000 ]
		lappend actions	[ EpCreateDialogAction "EpDialog4" ]
		lappend actions	[ EpCreateUnitMoveAction $npcGetg "RIGHT" ]
		lappend actions	[ EpCreateUnitMoveAction $npcGetg "RIGHT" ]
		lappend actions	[ EpCreateUnitMoveAction $npcGetg "RIGHT" ]
		lappend actions	[ EpCreateUnitMoveAction $npcGetg "RIGHT" ]
		lappend actions	[ EpCreateUnitMoveAction $npcGetg "RIGHT" ]
		set incident	[ EpCreateBlockingActionIncident $trigger 0 0 ]
		
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentFadeOut {} {
		global pHeroUnit

		set trigger		[ EpCreateUnitPositionTrigger	$pHeroUnit 31 77 31 77 ]
		set action		[ EpCreateFadeAction			out 1500 ]
		set incident	[ EpCreateIncident				$trigger $action 0 ]

		set incCount	[ EpRegisterIncident			$incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentFadeIn {} {
		global pHeroUnit

		set trigger		[ EpCreateUnitPositionTrigger	$pHeroUnit 29 77 29 77 ]
		set action		[ EpCreateFadeAction			in 3000 ]
		set incident	[ EpCreateIncident				$trigger $action 0 ]

		set incCount	[ EpRegisterIncident			$incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
	
	proc init { curWorld } {
		global world hero npcGetg npcGloop
		global pHeroUnit
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		set world					$curWorld;
		
		set pHeroUnit				[ createHero 30 77 ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpUnitSetColor			$pHeroUnit 255 0 255
		#EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		#EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		EpUnitSetArnMesh			$pHeroUnit "PoolGModel"
        
		set npcGetg					[ createEnemy 34 79 ];
		EpUnitSetColor			$npcGetg 128 128 0
		EpCharacterSetStat			$npcGetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcGetg -1
		EpEnemySetTalkable			$npcGetg 1
		EpUnitSetArnMesh			$npcGetg "PoolGModel"
		
		set npcGloop				[ createEnemy 30 82 ];
		EpUnitSetColor			$npcGloop 0 255 255
		EpCharacterSetStat			$npcGloop 3 1 4 1 1 1
		EpCharacterSetCurHp			$npcGloop -1
		EpEnemySetTalkable			$npcGloop 1
		EpUnitSetArnMesh			$npcGloop "PoolGModel"


		registerIncidentInitTalk
		registerIncidentFadeIn
		registerIncidentFadeOut
		
		createWarpPosition			"EpRoomWorld" 25 82
	}

	proc enter {} {
		EpOutputDebugString " - [info level 0] called\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

}
