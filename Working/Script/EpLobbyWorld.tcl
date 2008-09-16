namespace eval EpLobbyWorld {
	set modelFilePath "TestStage.arn"
	set dialogNameList [ list ]
	variable world
	
	proc init { curWorld } {
		variable world
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		set world				$curWorld;
	}
	
	proc enter {} {
		
		EpOutputDebugString " - [info level 0] called\n"
		
		set pHeroUnit				[ getHero { 43 79 } ];
		EpCharacterSetControllable	$pHeroUnit 1
		
		                                           # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat				$pHeroUnit    10     5     7       5      5      5
		EpCharacterSetCurHp				$pHeroUnit -1
		EpCharacterSetCurCs				$pHeroUnit -1
		EpCharacterSetMoveDuration		$pHeroUnit [expr 0.05]
		EpUnitSetColor					$pHeroUnit 255 128 255
		
		set curWorld					[ EpGetCurWorld ]
		set item						[ EpCreateStructureObjectPickable 41 81 ]
		EpUnitSetArnMesh				$item		"PushableBox"
		EpRegisterToWorld				$curWorld	$item
		
		set item						[ EpCreateStructureObjectPickable 44 81 ]
		EpUnitSetArnMesh				$item		"PushableBox"
		EpRegisterToWorld				$curWorld	$item
		
		EpRegisterInitFadeInIncident
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

}
