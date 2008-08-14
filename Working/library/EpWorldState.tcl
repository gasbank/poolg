namespace eval EpWorldState {
	variable pWorld
	
	proc init {pCurWorld} {
		EpOutputDebugString " - WorldState init Ptr: $pCurWorld\n"
		variable pWorld
		
		set pWorld				$pCurWorld;
	}

	
	proc enter {} {
		EpOutputDebugString " - WorldState enter\n"
		
		set pHeroUnit				[createHero -18 -4];
		set pEnemyUnit1				[createEnem -19 11];
		set pEnemyUnit2				[createEnem -9 8];
		set pEnemyUnit3				[createEnem -10 -11];
		set pEnemyUnit4				[createEnem 3 -6];
		set pEnemyUnit5				[createEnem 18 4];
		set pEnemyUnit6				[createEnem 13 -25];
		set pNPCUnit1				[createEnem 4 -26];
		set pTestEnemyUnit			[createEnem 4 -23];
		
		EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		EpCharacterSetMaxAndCurHp		$pHeroUnit 500 -1
		EpCharacterSetMoveDuration		$pHeroUnit [expr 0.1]
		EpCharacterSetColor			$pHeroUnit 255 0 255
		
		EpCharacterSetColor			$pEnemyUnit1 128 128 0
		EpCharacterSetMaxAndCurHp		$pEnemyUnit1 2 -1
		
		EpCharacterSetColor			$pEnemyUnit2 0 255 255
		EpCharacterSetMaxAndCurHp		$pEnemyUnit2 4 -1
		
		EpCharacterSetColor			$pEnemyUnit3 255 255 0
		EpCharacterSetMaxAndCurHp		$pEnemyUnit3 6 -1

		EpCharacterSetColor			$pEnemyUnit4 0 128 128
		EpCharacterSetMaxAndCurHp		$pEnemyUnit4 8 -1

		EpCharacterSetColor			$pEnemyUnit5 128 0 128
		EpCharacterSetMaxAndCurHp		$pEnemyUnit5 10 -1

		EpCharacterSetColor			$pEnemyUnit6 0 0 0
		EpCharacterSetMaxAndCurHp		$pEnemyUnit6 12 -1
		
		EpCharacterSetColor			$pNPCUnit1 0 255 0
		
		# EpCharacterSetTalkable		$pNPCUnit1 1
		# EpCharacterSetRandomWalkable		$pNPCUnit1 1
		# EpCharacterSetBoundary		$pNPCUnit1 33 20 35 18  # left, top, right, bottom

		# Setting Test Enemy Unit
		EpCharacterSetColor			$pTestEnemyUnit 255 255 255
		EpEnemySetRandomWalkable		$pTestEnemyUnit	1
		EpCharacterSetBoundary			$pTestEnemyUnit 3 -22 5 -25  # left, top, right, bottom
		EpEnemySetTalkable			$pTestEnemyUnit	1
	}
	
	proc leave {} {
		EpOutputDebugString " - WorldState leave\n"
	}

	proc printDialogText {} {
		EpWriteDialog "ÇÑ±ÛÁ«¶óarlsdgklgtulsdu;asjd;tjhklsaerlushldfhlzxcvhxhjzvbkjzxcfjha;srhqwieutpuiwetp;oiweoptyiisauhljkdflkhzx"
	}

	# Not used procedure
	proc createChar { tileX tileY ctrl } {
		variable pWorld
		set unit [ EpCreateCharacter $tileX $tileY $ctrl ];
		EpRegisterToWorld $pWorld $unit
		return $unit
	}

	proc createHero { tileX tileY } {
		variable pWorld
		set unit [ EpCreateHero $tileX $tileY ];
		EpRegisterToWorld $pWorld $unit
		return $unit
	}

	proc createEnem { tileX tileY } {
		variable pWorld
		set unit [ EpCreateEnemy $tileX $tileY ];
		EpRegisterToWorld $pWorld $unit
		return $unit
	}	
}
