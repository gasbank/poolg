namespace eval EpCeilingWorld {
	set modelFilePath	"CeilingWorld.arn"
	set dialogNameList [ list introDialog GlooPDialog GetGDialog SetGDialog ]
	global world hero npcGetg npcGloop pHeroUnit npcSetg

	proc registerIncidentInitTalk {} {
		global world hero npcGetg npcGloop pHeroUnit
		
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 30 77 30 77 0x100 ]
		set actions		[ EpCreateDialogAction "EpCeilingWorld::introDialog" ]
				
		set incident	[ EpCreateBlockingActionIncident $trigger 0 1 ]
		
		EpIncidentSetName $incident "Init Talk"
		
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc Quest1 {} {
		global world hero npcGetg npcGloop pHeroUnit
	
		set trigger		[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcGetg 0x001 ]
		set actions		[ EpCreateDialogAction "EpCeilingWorld::GetGDialog" ]
		lappend actions		[ EpCreateStartIncidentAction [ EpCeilingWorld::Quest2 ] ]
		lappend actions		[ EpCreateStartIncidentAction [ EpCeilingWorld::Quest3 ] ]
		
		set incident	[ EpCreateBlockingActionIncident $trigger 0 -1 ]

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Quest1 incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc Quest2 {} {
		global world hero npcGetg npcGloop pHeroUnit

		set trigger	 [ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcGloop 0x001 ]
		set actions	 [ EpCreateDialogAction "EpCeilingWorld::GlooPDialog" ]

		set incident	 [ EpCreateBlockingActionIncident $trigger 0 -1 ]
		EpIncidentSetName	$incident "Quest2 incident"
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		## --------------------------------------------
		## Do not register a incident planned
		## to be invoked by EpCreateStartIncidentAndWaitAction!!!
		## --------------------------------------------
		#set incCount	[ EpRegisterIncident $incident ]
		#EpOutputDebugString " - Incident count: $incCount\n"

		EpOutputDebugString " - Quest2 Incident returned $incident\n"
		return $incident
	}

	proc Quest3 {} {
		global world hero npcGetg npcGloop pHeroUnit npcSetg

		set trigger	 [ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcSetg 0x001 ]
		set action	 [ EpCreateDialogAction "EpCeilingWorld::SetGDialog" ]
		set incident	 [ EpCreateBlockingActionIncident $trigger $action -1 ]
		EpIncidentSetName	$incident "Quest3 incident"
		## --------------------------------------------
		## Do not register a incident planned
		## to be invoked by EpCreateStartIncidentAndWaitAction!!!
		## --------------------------------------------
		##set incCount	[ EpRegisterIncident $incident ]
		##EpOutputDebugString " - Incident count: $incCount\n"
		
		EpOutputDebugString " - Quest3 Incident returned $incident\n"
		return $incident
	}

	proc init { curWorld } {
		global world hero npcGetg npcGloop npcSetg
		global pHeroUnit
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		set world					$curWorld;
		
		set pHeroUnit				[ createHero 30 77 ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpUnitSetColor				$pHeroUnit 255 0 255
		#EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		#EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		EpUnitSetArnMesh			$pHeroUnit "PoolGModel"
        
		set npcGetg					[ createEnemy 34 79 ];
		EpUnitSetColor				$npcGetg 128 128 0
		EpCharacterSetStat			$npcGetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcGetg -1
		EpEnemySetTalkable			$npcGetg 1
		EpUnitSetArnMesh			$npcGetg "PoolGModel"
		
		set npcGloop				[ createEnemy 30 82 ];
		EpUnitSetColor				$npcGloop 0 255 255
		EpCharacterSetStat			$npcGloop 3 1 4 1 1 1
		EpCharacterSetCurHp			$npcGloop -1
		EpEnemySetTalkable			$npcGloop 1
		EpUnitSetArnMesh			$npcGloop "PoolGModel"

		set npcSetg					[ createEnemy 30 72 ];
		EpUnitSetColor				$npcSetg 128 128 0
		EpCharacterSetStat			$npcSetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcSetg -1
		EpEnemySetTalkable			$npcSetg 1
		EpUnitSetArnMesh			$npcSetg "PoolGModel"


		registerIncidentInitTalk
		Quest1
			
		createWarpPosition			"EpRoomWorld" 25 82
	}

	proc enter {} {
		EpOutputDebugString " - [info level 0] called\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}




	namespace eval introDialog {
		
		set region [ list 0 0 -1 -1 ]; ;# left, top, right, bottom
		set oneTime 1;
		
		set player "PoolG"
		set npc1 "GlooP"
		set npc2 "GetG"
		
		set dialog [ list\
			$npc1		"PoolC에서 대청소를 해버리는 바람에, 우리들의 삶의 터전이 좁아저 버렸다네"\
			$player		"뭬야!"\
			$npc1		"게다가 앞으론 우리들을 살 수 없도록 언제나 깨끗하게 유지하겠다고 하더군"\
			$player		"맙소사! 난 가만히 있을 수 없어"\
			$npc2		"제발 그들을 말려주게"\
			$player		"이대로 가만히 있을 순 없지. 다녀오겠네"\
		];
	}

	namespace eval GlooPDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GlooP"
	
		set dialog [ list\
			$npc		"동아리 방에는 '괭이' 라고 불리는 생물이 돌아다니는 듯 하네"\
			$player		"뭐하는 녀석인가?"\
			$npc		"KKK라고 불리는 사람이 그린 고양이라고 하던데...."\
			$player		"그림이 돌아다닐리가 없잖아"\
			$npc		"SetG에게 물어보시게"\
		];	
	}

	namespace eval GetGDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GetG"
	
		set dialog [ list\
			$npc		"변수는 Private에 넣은뒤, Get 함수로 불러온다면 누가 불러가는지 추적이 편해진다네"\
			$player		"그것은 코딩의 기초가 아닌가"\
			$npc		"하지만 이건 C에선 지원하지 않지"\
			$player		"도대체 왜 아직도 C를 쓰는지 모르겠어"\
			$npc		"GlooP에게 물어보시게"\
		];	
	}

	namespace eval SetGDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "SetG"
	
		set dialog [ list\
			$npc		"안녕"\
			$player		"어쩌라구요♡"\
		];	
	}



}
