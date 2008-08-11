package require Thread

namespace eval EpWorldState {
	variable pWorld
	variable pHeroUnit pEnemyUnit1 pEnemyUnit2 pEnemyUnit3
	
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
		variable pEnemyUnit3
		variable pWorld
		
		set pHeroUnit				[EpCreateCharacter 13 13 1];
		set pEnemyUnit1				[EpCreateCharacter 23 23 0];
		set pEnemyUnit2				[EpCreateCharacter  3  3 0];
		set pEnemyUnit3				[EpCreateCharacter 18 -1 0];
		set curUnitCount			[EpRegisterToWorld $pWorld $pHeroUnit];
		set curUnitCount			[EpRegisterToWorld $pWorld $pEnemyUnit1];
		set curUnitCount			[EpRegisterToWorld $pWorld $pEnemyUnit2];
		set curUnitCount			[EpRegisterToWorld $pWorld $pEnemyUnit3];
		
		
		EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		EpCharacterSetMaxAndCurHp	$pHeroUnit 50 50
		EpCharacterSetMoveDuration	$pHeroUnit 0.25
		
		EpOutputDebugString " -- Current Unit Count: $curUnitCount\n"
		
		# Thread testing
		if 0 {
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
