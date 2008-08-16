namespace eval EpA213World {
	set modelFilePath	"Worlds/A213.arn"
	
	proc init { pCurWorld } {
		variable world
		
		EpOutputDebugString " - EpA213World init Ptr: $pCurWorld\n"
		
		set world				$pCurWorld;
	}

	proc enter {} {

	}
	
	proc leave {} {
		EpOutputDebugString " - EpA213World leave\n"
	}

}
