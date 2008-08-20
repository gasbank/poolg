namespace eval EpCeilingWorld {
	set modelFilePath	"CeilingWorld.arn"
	set dialogNameList [ list introDialog GlooPDialog GetGDialog ]
	global world hero npcGetg npcGloop pHeroUnit

	proc registerIncidentInitTalk {} {
		global world hero npcGetg npcGloop pHeroUnit
		
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 30 77 30 77 0x100 ]
		set actions		[ EpCreateDialogAction "EpCeilingWorld::introDialog" ]
				
		set incident	[ EpCreateBlockingActionIncident $trigger 0 0 ]
		
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc QuestTalk {} {
		global world hero npcGetg npcGloop pHeroUnit
	
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 33 80 35 78 0x101 ]
		set actions		[ EpCreateDialogAction "EpCeilingWorld::GetGDialog" ]
		set actions		[ EpCreateScriptAction "EpCeilingWorld::Quest" ]
		#set actions		[ EpCreateDialogAction "EpCeilingWorld::GetGDialog2" ]
				
		set incident	[ EpCreateBlockingActionIncident $trigger 0 0 ]

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc Quest {} {
		global world hero npcGetg npcGloop pHeroUnit

		set questtrigger	 [ EpCreateUnitPositionTrigger $pHeroUnit 29 83 31 81 0x101 ]
		set questaction		 [ EpCreateDialogAction "EpCeilingWorld::GlooPDialog" ]
		set questincident	 [ EpCreateBlockingActionIncident $questtrigger $questaction 0 ]

		set incCount	[ EpRegisterIncident $questincident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc init { curWorld } {
		global world hero npcGetg npcGloop
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


		registerIncidentInitTalk
		QuestTalk
			
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
			$npc		"어쩌겠나. 08 프로젝트 팀에게 문의를 하시게"\
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
			$npc		"어쩌겠나. 08 프로젝트 팀에게 문의를 하시게"\
		];	
	}

	namespace eval GetGDialog2 {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GetG"
	
		set dialog [ list\
			$npc		"빠르군"\
		];	
	}



}
