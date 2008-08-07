package require Thread

namespace eval EpWorldState {
	


	variable pWorld			0
	variable pHeroUnit		0
	variable pEnemyUnit		1
	
	proc init {pCurWorld} {
		EpOutputDebugString " - WorldState init Ptr: $pCurWorld\n"
		
		variable pHeroUnit
		variable pEnemyUnit
		variable pWorld
		
		set pWorld				$pCurWorld;
		
		thread::create {
			puts "xxx"
			variable cx 0
			proc passTheTime { } {
				variable cx
				puts "cx: $cx\n"
				incr cx
				after 1000 passTheTime
			}
 			passTheTime
			vwait forever
		}
	}

	proc enter {} {
		EpOutputDebugString " - WorldState enter\n"
		
		variable pHeroUnit
		variable pEnemyUnit		
		variable pWorld
		
		set pHeroUnit			[EpCreateUnit 0 0 1];
		set pEnemyUnit			[EpCreateUnit 7 0 0];
		set curUnitCount		[EpRegisterToWorld $pWorld $pHeroUnit];
		set curUnitCount		[EpRegisterToWorld $pWorld $pEnemyUnit];
		
		EpUnitSetRotX $pHeroUnit [ToRadian -90]
		EpUnitSetRotZ $pHeroUnit [ToRadian 90]
		EpUnitSetPosZ $pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		
		EpOutputDebugString " -- Current Unit Count: $curUnitCount\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - WorldState leave\n"
		
		EpReleaseUnit $pHeroUnit
		EpReleaseUnit $pEnemyUnit
	}

}
