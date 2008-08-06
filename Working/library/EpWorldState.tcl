namespace eval EpWorldState {
	
	variable heroUnit 0x0
	
	proc init {} {
		EpOutputDebugString " - WorldState init\n"
		
		set heroUnit [EpCreateUnit 0, 0, 0];
	}

	proc enter {} {
		EpOutputDebugString " - WorldState enter\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - WorldState leave\n"
	}

}
