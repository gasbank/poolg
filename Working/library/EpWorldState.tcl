package require Thread

namespace eval EpWorldState {
	variable pWorld
	variable pHeroUnit pEnemyUnit1 pEnemyUnit2
	
	proc init {pCurWorld} {
		EpOutputDebugString " - WorldState init Ptr: $pCurWorld\n"
		variable pWorld
		variable pHeroUnit pEnemyUnit1 pEnemyUnit2
		
		set pWorld				$pCurWorld;
	}

	proc enter {} {
		EpOutputDebugString " - WorldState enter\n"
		
		variable pHeroUnit
		variable pEnemyUnit1
		variable pEnemyUnit2		
		variable pWorld
		
		set pHeroUnit			[EpCreateUnit 0 0 1];
		set pEnemyUnit1			[EpCreateUnit 7 0 0];
		set pEnemyUnit2			[EpCreateUnit 10 -10 0];
		set curUnitCount		[EpRegisterToWorld $pWorld $pHeroUnit];
		set curUnitCount		[EpRegisterToWorld $pWorld $pEnemyUnit1];
		set curUnitCount		[EpRegisterToWorld $pWorld $pEnemyUnit2];
		
		EpUnitSetRotX			$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ			$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ			$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		EpUnitSetMoveDuration	$pHeroUnit 0.25
		
		EpOutputDebugString " -- Current Unit Count: $curUnitCount\n"
		
		# Thread testing
		thread::create {
			puts "xxx"
			variable cx 0
			proc passTheTime { } {
				variable cx
				puts "cx: $cx"
				incr cx
				after 1000 passTheTime
			}
 			passTheTime
			vwait forever
		}
	}
	
	proc leave {} {
		EpOutputDebugString " - WorldState leave\n"
		
		EpReleaseUnit $pHeroUnit
		EpReleaseUnit $pEnemyUnit1
		EpReleaseUnit $pEnemyUnit2
	}


	proc printDialogText {} {
		EpWriteDialog "ÇÑ±ÛÁ«¶óarlsdgklgtulsdu;asjd;tjhklsaerlushldfhlzxcvhxhjzvbkjzxcfjha;srhqwieutpuiwetp;oiweoptyiisauhljkdflkhzx"
	}
}
