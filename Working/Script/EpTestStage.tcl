namespace eval EpTestStage {
	set modelFilePath "TestStage.arn"
	set dialogNameList [ list  ]
	global world hero
	
	proc init { curWorld } {
		global world hero
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		set world				$curWorld;
	}
	
	proc enter {} {
		global world hero
		
		EpOutputDebugString " - [info level 0] called\n"
		
		set pHeroUnit				[ createHero 43 79 ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.05]
		EpUnitSetColor				$pHeroUnit 255 128 255
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

}
