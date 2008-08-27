source "Script/EpCeilingWorld_FirstMeetingQuest.tcl"

namespace eval EpCeilingWorld {
	set modelFilePath	"CeilingWorld.arn"
	set dialogNameList [ list introDialog GetGDialog SetGDialog ]
	global world hero npcGetg npcGloop npcSetg weakCat
	variable pHeroUnit
	variable startPos { 30 77 }
	variable mainQuest1_incident
	variable mainQuest2_incident	

	proc init { curWorld } {
		global world hero npcGetg npcGloop npcSetg weakCat
		variable pHeroUnit
		variable startPos
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		set world					$curWorld;
		
		set pHeroUnit				[ getHero $startPos ];
        
		set npcGetg					[ createEnemy 34 79 ];
		EpUnitSetColor				$npcGetg 128 128 0
		EpCharacterSetStat			$npcGetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcGetg -1
		EpEnemySetTalkable			$npcGetg 1
		EpUnitSetArnMesh			$npcGetg "PoolGModel"
		EpUnitSetName				$npcGetg "GetG"
		EpUnitSetNameVisible		$npcGetg 1		
		
		set npcSetg					[ createEnemy 30 72 ];
		EpUnitSetColor				$npcSetg 128 128 0
		EpCharacterSetStat			$npcSetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcSetg -1
		EpEnemySetTalkable			$npcSetg 1
		EpUnitSetArnMesh			$npcSetg "PoolGModel"
		EpUnitSetName				$npcSetg "SetG"
		EpUnitSetNameVisible		$npcSetg 1

		registerIncidentInitTalk

		FirstMeetingQuest::register
			
		createWarpPosition			"EpRoomWorld" STOP 25 82
	}

	proc enter {} {
		EpOutputDebugString " - [info level 0] called\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

	proc registerIncidentInitTalk {} {
		global world hero npcGetg npcGloop
		variable pHeroUnit
		
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

	proc Quest4 {} {
		global world hero npcGetg npcGloop npcSetg
		variable pHeroUnit

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
			$npc1		"������ ���� ������ �� ������ �ͺ��ڳ�?"\
			$npc1		"���� �� ��Ⱑ �ֳ�"\
			$player		"�˰ڳ�"\
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
}
