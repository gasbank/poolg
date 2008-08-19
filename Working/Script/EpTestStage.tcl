namespace eval EpTestStage {
	set modelFilePath "TestStage.arn"
	global world hero
	
	proc init { curWorld } {
		global world hero
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		set world				$curWorld;
	}
	
	proc registerIncidentSwitch {} {
		global world hero
		
		set trigger		[ EpCreateUnitPositionTrigger $TestStObject 50 39 -1 -1 ]
		set action		[ EpCreateScriptAction "EpSetDoAnim [EpGetNode Door] 1" ]
		set incident		[ EpCreateIncident $trigger $action 1 ]

		set action2		[ EpCreateScriptAction "EpSetDoAnim [EpGetNode TofuMan] 1" ]
		
		EpAddActionToIncident $incident $action2
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc enter {} {
		global world hero
		
		EpOutputDebugString " - [info level 0] called\n"
		
		set pHeroUnit				[ createHero 45 60 ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpUnitSetColor				$pHeroUnit 255 0 255
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

}
