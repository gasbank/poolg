namespace eval EpWorldState {
	variable pWorld
	
	proc init {pCurWorld} {
		EpOutputDebugString " - WorldState init Ptr: $pCurWorld\n"
		variable pWorld
		
		set pWorld				$pCurWorld;
	}

	
	proc enter {} {
		EpOutputDebugString " - WorldState enter\n"
		
		set pHeroUnit				[createHero 26 74];
		set pEnemyUnit1				[createEnem 25 90];
		set pEnemyUnit2				[createEnem 37 88];
		set pEnemyUnit3				[createEnem 35 67];
		set pEnemyUnit4				[createEnem 47 73];
		set pEnemyUnit5				[createEnem 62 83];
		set pEnemyUnit6				[createEnem 59 58];
		set pEnemyUnit7				[createEnem 36 53];
		set pEnemyUnit8				[createEnem 42 44];
		set pNPCUnit1				[createEnem 47 65];
		set pTestEnemyUnit			[createEnem 55 40];
		
		EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
                                                                  # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit   30     20    20     10     10     10
		EpCharacterSetCurHp			$pHeroUnit -1
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration		$pHeroUnit [expr 0.2]
		EpCharacterSetColor			$pHeroUnit 255 0 255
		
		
		EpCharacterSetColor			$pEnemyUnit1 128 128 0
		EpCharacterSetStat			$pEnemyUnit1 4 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit1 -1
		
		EpCharacterSetColor			$pEnemyUnit2 0 255 255
		EpCharacterSetStat			$pEnemyUnit2 6 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit2 -1
		
		EpCharacterSetColor			$pEnemyUnit3 255 255 0
		EpCharacterSetStat			$pEnemyUnit3 5 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit3 -1

		EpCharacterSetColor			$pEnemyUnit4 0 128 128
		EpCharacterSetStat			$pEnemyUnit4 5 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit4 -1

		EpCharacterSetColor			$pEnemyUnit5 128 0 128
		EpCharacterSetStat			$pEnemyUnit5 4 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit5 -1

		EpCharacterSetColor			$pEnemyUnit6 64 64 0
		EpCharacterSetCurHp			$pEnemyUnit6 -1
		EpCharacterSetStat			$pEnemyUnit6 4 1 1 1 1 1

		EpCharacterSetColor			$pEnemyUnit7 0 64 64
		EpCharacterSetStat			$pEnemyUnit7 6 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit7 -1

		EpCharacterSetColor			$pEnemyUnit8 0 0 0
		EpCharacterSetStat			$pEnemyUnit8 4 1 1 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit8 -1
		
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
