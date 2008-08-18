namespace eval EpA213World {
	set modelFilePath	"A213World.arn"
	
	proc init { curWorld } {
		variable world
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		
		set world				$curWorld;
	}

	proc enter {} {
		EpOutputDebugString " - [info level 0] called\n"
		
		set pHeroUnit				[ createHero 45 60 ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpCharacterSetColor			$pHeroUnit 255 0 255
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

}
