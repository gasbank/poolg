namespace eval EpWorldState {
	variable pWorld
	
	proc init {pCurWorld} {
		EpOutputDebugString " - WorldState init Ptr: $pCurWorld\n"
		variable pWorld
		
		set pWorld				$pCurWorld;
	}

	
	proc enter {} {
		EpOutputDebugString " - WorldState enter\n"
		
		set pHeroUnit				[createChar 32 32 1];
		set pEnemyUnit1				[createChar 40 14 0];
		set pEnemyUnit2				[createChar 45 14 0];
		set pEnemyUnit3				[createChar 30 22 0];
		set pNPCUnit1				[createChar 34 19 0];
		
		EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		EpCharacterSetMaxAndCurHp	$pHeroUnit 500 -1
		EpCharacterSetMoveDuration	$pHeroUnit 0.25
		EpCharacterSetColor			$pHeroUnit 255 0 255
		
		EpCharacterSetColor			$pEnemyUnit1 128 128 0
		EpCharacterSetMaxAndCurHp	$pEnemyUnit1 2 -1
		
		EpCharacterSetColor			$pEnemyUnit2 0 255 255
		EpCharacterSetMaxAndCurHp	$pEnemyUnit2 4 -1
		
		EpCharacterSetColor			$pEnemyUnit3 255 255 0
		EpCharacterSetMaxAndCurHp	$pEnemyUnit3 6 -1
		
		EpCharacterSetColor			$pNPCUnit1 0 255 0
		EpCharacterSetTalkable		$pNPCUnit1 1
	}
	
	proc leave {} {
		EpOutputDebugString " - WorldState leave\n"
	}


	proc printDialogText {} {
		EpWriteDialog "ÇÑ±ÛÁ«¶óarlsdgklgtulsdu;asjd;tjhklsaerlushldfhlzxcvhxhjzvbkjzxcfjha;srhqwieutpuiwetp;oiweoptyiisauhljkdflkhzx"
	}
	proc createChar { tileX tileY ctrl } {
		variable pWorld
		set unit [ EpCreateCharacter $tileX $tileY $ctrl ];
		EpRegisterToWorld $pWorld $unit
		return $unit
	}
	
}
