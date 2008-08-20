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
		EpCharacterSetStat				$pHeroUnit    10     5     7       5      5      5
		EpCharacterSetCurHp				$pHeroUnit -1
		EpCharacterSetCurCs				$pHeroUnit -1
		EpCharacterSetMoveDuration		$pHeroUnit [expr 0.05]
		EpUnitSetColor					$pHeroUnit 255 128 255
		
		set testEnemy					[createEnemy 43 85 1];
		
		EpCharacterSetStat				$testEnemy 8 1 3 1 1 1
		EpCharacterSetCurHp				$testEnemy -1
		EpEnemySetRandomWalkable		$testEnemy 1
		
		
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

}
