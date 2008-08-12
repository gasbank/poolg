package require Thread

namespace eval EpWorldState {
	variable pWorld
	variable pHeroUnit pEnemyUnit1 pEnemyUnit2 pEnemyUnit3 pNPCUnit1;
	
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
		variable pNPCUnit1
		variable pWorld
		
		set pHeroUnit				[EpCreateCharacter 30 14 1];
		set pEnemyUnit1				[EpCreateCharacter 40 14 0];
		set pEnemyUnit2				[EpCreateCharacter 45 14 0];
		set pEnemyUnit3				[EpCreateCharacter 30 22 0];
		set pNPCUnit1				[EpCreateCharacter 34 19 0];
		set curUnitCount			[EpRegisterToWorld $pWorld $pHeroUnit];
		set curUnitCount			[EpRegisterToWorld $pWorld $pEnemyUnit1];
		set curUnitCount			[EpRegisterToWorld $pWorld $pEnemyUnit2];
		set curUnitCount			[EpRegisterToWorld $pWorld $pEnemyUnit3];
		set curUnitCount			[EpRegisterToWorld $pWorld $pNPCUnit1];
		
		EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		EpCharacterSetMaxAndCurHp	$pHeroUnit 500 500
		EpCharacterSetMaxAndCurHp	$pEnemyUnit1 2 2
		EpCharacterSetMaxAndCurHp	$pEnemyUnit2 2 2
		EpCharacterSetMaxAndCurHp	$pEnemyUnit3 2 2
		EpCharacterSetMoveDuration	$pHeroUnit 0.25
		EpCharacterSetColor		$pNPCUnit1 255 0 0
		EpCharacterSetTalkable		$pNPCUnit1 1
		
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
		EpReleaseUnit $pEnemyUnit3
		EpReleaseUnit $pNPCUnit1
	}


	proc printDialogText {} {
		EpWriteDialog "ÇÑ±ÛÁ«¶óarlsdgklgtulsdu;asjd;tjhklsaerlushldfhlzxcvhxhjzvbkjzxcfjha;srhqwieutpuiwetp;oiweoptyiisauhljkdflkhzx"
	}
}
