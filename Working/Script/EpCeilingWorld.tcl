namespace eval EpCeilingWorld {
	set modelFilePath	"ceiling.arn"
	variable world hero npcGetg npcGloop
	
	proc init { curWorld } {
		variable world hero npcGetg npcGloop
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		set world					$curWorld;
		
		set pHeroUnit				[ createHero 30 77 ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpCharacterSetColor			$pHeroUnit 255 0 255
		EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
        
		set npcGetg					[ createEnemy 34 79 ];
		EpCharacterSetColor			$npcGetg 128 128 0
		EpCharacterSetStat			$npcGetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcGetg -1
		
		set npcGloop				[ createEnemy 30 82 ];
		EpCharacterSetColor			$npcGloop 0 255 255
		EpCharacterSetStat			$npcGloop 3 1 4 1 1 1
		EpCharacterSetCurHp			$npcGloop -1
		
		
		createWarpPosition			"EpRoomWorld" 25 82
		
	}

	proc enter {} {
		EpOutputDebugString " - [info level 0] called\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

}
