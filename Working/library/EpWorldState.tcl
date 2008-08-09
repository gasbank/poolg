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
		
		set pHeroUnit				[EpCreateCharacter 0 -24 1];
		set pEnemyUnit1				[EpCreateCharacter 7 0 0];
		set pEnemyUnit2				[EpCreateCharacter 10 -10 0];
		set pEnemyUnit3				[EpCreateCharacter 0 -30 0];
		set curUnitCount			[EpRegisterToWorld $pWorld $pHeroUnit];
		set curUnitCount			[EpRegisterToWorld $pWorld $pEnemyUnit1];
		set curUnitCount			[EpRegisterToWorld $pWorld $pEnemyUnit2];
		set curUnitCount			[EpRegisterToWorld $pWorld $pEnemyUnit3];
		
		EpCharacterSetMaxAndCurHp	$pHeroUnit 50 50
		EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		EpUnitSetMoveDuration		$pHeroUnit 0.25
		
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
		EpWriteDialog "�ѱ�����arlsdgklgtulsdu;asjd;tjhklsaerlushldfhlzxcvhxhjzvbkjzxcfjha;srhqwieutpuiwetp;oiweoptyiisauhljkdflkhzx"
	}
}
