namespace eval EpCeilingWorld {
	set modelFilePath	"CeilingWorld.arn"
	set dialogNameList [ list introDialog GlooPDialog rGlooPDialog GlooPDialog2 GetGDialog SetGDialog weakCatDialog catDeadDialog ]
	global world hero npcGetg npcGloop pHeroUnit npcSetg weakCat
	variable startPos { 30 77 }
	variable mainQuest1_incident
	variable mainQuest2_incident

	proc registerIncidentInitTalk {} {
		global world hero npcGetg npcGloop pHeroUnit
		
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 30 77 30 77 0x100 ]
		set actions		[ EpCreateFadeAction "in" 3000 ]
		lappend actions [ EpCreateDialogAction "EpCeilingWorld::introDialog" ]
				
		set incident	[ EpCreateBlockingActionIncident $trigger 0 1 ]
		
		EpIncidentSetName $incident "Init Talk"
		EpIncidentSetControlDuringAction $incident 0
		
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "InitTalk incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc mainQuest {} {
		global world hero npcGetg npcGloop pHeroUnit npcSetg weakCat
		variable mainQuest1_incident
		variable mainQuest2_incident

		set incident	[ EpCreateSequentialIncident 1 ]

		EpAddTriggerToSequence	$incident	[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcGloop 0x001 ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::GlooPDialog" ]
		
		EpAddTriggerToSequence	$incident	[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $weakCat 0x001 ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::weakCatDialog" ]
		EpAddActionToSequence	$incident	[ EpCreateScriptAction "EpEnemySetTalkable $weakCat 0" ]

		EpAddTriggerToSequence	$incident	[ EpRemoveFlagTrigger $weakCat ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::catDeadDialog" ]

		EpAddTriggerToSequence	$incident	[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcGloop 0x001 ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::GlooPDialog2" ]

		EpIncidentSetName	$incident "mainQuest incident"
		
		set incCount	[ EpRegisterSequentialIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc Quest3 {} {
		global world hero npcGetg npcGloop pHeroUnit

		set trigger	 [ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcGloop 0x001 ]
		set actions	 [ EpCreateDialogAction "EpCeilingWorld::GlooPDialog" ]

		set incident	 [ EpCreateBlockingActionIncident $trigger 0 -1 ]
		EpIncidentSetName	$incident "Quest3 incident"
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Quest2 incident"

		## --------------------------------------------
		## Do not register a incident planned
		## to be invoked by EpCreateStartIncidentAndWaitAction!!!
		## --------------------------------------------
		#set incCount	[ EpRegisterIncident $incident ]
		#EpOutputDebugString " - Incident count: $incCount\n"

		EpOutputDebugString " - Quest2 Incident returned $incident\n"
		return $incident
	}

	proc Quest4 {} {
		global world hero npcGetg npcGloop pHeroUnit npcSetg

		set trigger			[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcSetg 0x001 ]
		set action			[ EpCreateDialogAction "EpCeilingWorld::SetGDialog" ]
		set incident		[ EpCreateBlockingActionIncident $trigger $action -1 ]
		EpIncidentSetName	$incident "Quest4 incident"

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
		global world hero npcGetg npcGloop npcSetg pHeroUnit weakCat
		variable startPos
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		set world					$curWorld;
		
		set pHeroUnit				[ getHero $startPos ];
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
		EpUnitSetName				$npcGetg "GetG"
		EpUnitSetNameVisible		$npcGetg 1
		
		set npcGloop				[ createEnemy 30 82 ];
		EpUnitSetColor				$npcGloop 0 255 255
		EpCharacterSetStat			$npcGloop 3 1 4 1 1 1
		EpCharacterSetCurHp			$npcGloop -1
		EpEnemySetTalkable			$npcGloop 1
		EpUnitSetArnMesh			$npcGloop "PoolGModel"
		EpUnitSetName				$npcGloop "Gloop"
		EpUnitSetNameVisible		$npcGloop 1

		set npcSetg					[ createEnemy 30 72 ];
		EpUnitSetColor				$npcSetg 128 128 0
		EpCharacterSetStat			$npcSetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcSetg -1
		EpEnemySetTalkable			$npcSetg 1
		EpUnitSetArnMesh			$npcSetg "PoolGModel"
		EpUnitSetName				$npcSetg "SetG"
		EpUnitSetNameVisible		$npcSetg 1

		set weakCat					[ createEnemy 42 75 ];
		EpUnitSetColor				$weakCat 0 0 0 
		EpCharacterSetStat			$weakCat 2 2 2 2 2 2
		EpCharacterSetCurHp			$weakCat -1
		EpEnemySetTalkable			$weakCat 1
		EpUnitSetArnMesh			$weakCat "GwengYiModel"

		registerIncidentInitTalk
		mainQuest
			
		createWarpPosition			"EpRoomWorld" STOP 25 82
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
			$npc		"��~�⿡ ������ �ѹ� ����ð�"\
		];	
	}

	namespace eval rGlooPDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GlooP"
	
		set dialog [ list\
			$npc		"����� ����ϱ�?"\
			$player		"���� �ο�°� �ƴ϶�?"\
			$npc		"���׳�!"\
		];	
	}

	namespace eval GlooPDialog2 {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GlooP"
	
		set dialog [ list\
			$npc		"���߳�"\
			$player		"�� ��"\
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
			$npc		"GlooP���� ����ð�"\
		];	
	}

	namespace eval SetGDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "SetG"
	
		set dialog [ list\
			$npc		"�ȳ�"\
			$player		"��¼�󱸿䢽"\
		];	
	}

	namespace eval weakCatDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set enemy "GwengYi"
	
		set dialog [ list\
			$enemy		"���� �����? �� ����?"\
			$player		"..... ���� �̰�"\
			$enemy		"���!!!!!!!!!!!!!!!"\
			$player		"��, ����!"\
		];	
	}

	namespace eval catDeadDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
	
		set dialog [ list\
			$player		"����, �����ƴ�!"\
		];	
	}
}
