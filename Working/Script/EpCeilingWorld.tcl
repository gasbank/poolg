namespace eval EpCeilingWorld {
	set modelFilePath	"ceiling.arn"
	variable world hero npcGetg npcGloop
	
	proc init { curWorld } {
		variable world hero npcGetg npcGloop
		EpOutputDebugString " - CeilingWorld init Ptr: $curWorld\n"
		set world					$curWorld;
		
		set pHeroUnit				[ createHero 78 68 ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpCharacterSetColor			$pHeroUnit 255 0 255
		EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
        
		set npcGetg					[ createEnemy 70 68 ];
		EpCharacterSetColor			$npcGetg 128 128 0
		EpCharacterSetStat			$npcGetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcGetg -1
		
		set npcGloop				[ createEnemy 72 72 ];
		EpCharacterSetColor			$npcGloop 0 255 255
		EpCharacterSetStat			$npcGloop 3 1 4 1 1 1
		EpCharacterSetCurHp			$npcGloop -1
	}

	proc enter {} {
		EpOutputDebugString " - [info level 0] called\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

	proc createHero { tileX tileY } {
		variable world
		set unit [ EpCreateHero $tileX $tileY ];
		EpRegisterToWorld $world $unit
		return $unit
	}

	proc createEnemy { tileX tileY } {
		variable world
		set unit [ EpCreateEnemy $tileX $tileY ];
		EpRegisterToWorld $world $unit
		return $unit
	}	
}
