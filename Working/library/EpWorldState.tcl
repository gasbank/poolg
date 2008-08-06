namespace eval EpWorldState {
	variable pWorld			0
	variable pHeroUnit		0
	
	proc init {pCurWorld} {
		EpOutputDebugString " - WorldState init Ptr: $pCurWorld\n"
		
		variable pHeroUnit
		variable pWorld
		
		set pWorld				$pCurWorld;
	}

	proc enter {} {
		EpOutputDebugString " - WorldState enter\n"
		
		variable pHeroUnit
		variable pWorld
		
		set pHeroUnit			[EpCreateUnit 3 4 1];
		set curUnitCount		[EpRegisterToWorld $pWorld $pHeroUnit];
		
		EpOutputDebugString " -- Current Unit Count: $curUnitCount\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - WorldState leave\n"
		
		EpReleaseUnit $pHeroUnit
	}

}
