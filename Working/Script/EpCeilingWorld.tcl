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
			$npc1		"PoolC���� ��û�Ҹ� �ع����� �ٶ���, �츮���� ���� ������ ������ ���ȴٳ�"\
			$player		"����!"\
			$npc1		"�Դٰ� ������ �츮���� �� �� ������ ������ �����ϰ� �����ϰڴٰ� �ϴ���"\
			$player		"���һ�! �� ������ ���� �� ����"\
			$npc2		"���� �׵��� �����ְ�"\
			$player		"�̴�� ������ ���� �� ����. �ٳ���ڳ�"\
		];
	}

	namespace eval GlooPDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GlooP"
	
		set dialog [ list\
			$npc		"���Ƹ� �濡�� '����' ��� �Ҹ��� ������ ���ƴٴϴ� �� �ϳ�"\
			$player		"���ϴ� �༮�ΰ�?"\
			$npc		"KKK��� �Ҹ��� ����� �׸� ����̶�� �ϴ���...."\
			$player		"�׸��� ���ƴٴҸ��� ���ݾ�"\
			$npc		"��¼�ڳ�. 08 ������Ʈ ������ ���Ǹ� �Ͻð�"\
		];	
	}

	namespace eval GetGDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GetG"
	
		set dialog [ list\
			$npc		"������ Private�� ������, Get �Լ��� �ҷ��´ٸ� ���� �ҷ������� ������ �������ٳ�"\
			$player		"�װ��� �ڵ��� ���ʰ� �ƴѰ�"\
			$npc		"������ �̰� C���� �������� ����"\
			$player		"����ü �� ������ C�� ������ �𸣰ھ�"\
			$npc		"��¼�ڳ�. 08 ������Ʈ ������ ���Ǹ� �Ͻð�"\
		];	
	}

	namespace eval GetGDialog2 {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GetG"
	
		set dialog [ list\
			$npc		"������"\
		];	
	}



}
