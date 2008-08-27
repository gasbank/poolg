# Not Implemented Yet
#
# EpRoomWorld_EnterDoofuManQuest.tcl
#
# Goal:
# ������ ���� ���̸� óġ�ϰ� �κθ����κ��� ���� �Լ�
#
# NPCs:
# ����
#
# Objects:
# ����
#
# Description:
# �÷��̾�� ���̸� �����鼭 ���� �ؾ��Ѵٴ� ����� �����Ѵ�. ������ ���� ���̸�
# óġ�ϸ� �쿬ġ �ʰ� �κΰ� �湮�� ���� ���� ��ȭ�� ���۵ȴ�. ��ȭ�� ���ؼ�
# �÷��̾�� Dark Lord Choi�� A213�� �ִٴ� ������ �Լ��ϰ� �� ������ ������.
#
# Detail:
# 1. (25, 91) ~ (29, 91) �� ����� �����鼭 Ǯ��� ���̸� �����鼭 ���� Ǯ��� �ڴٰ� �����Ѵ�.
# 2. ���̰� 4���� ������ �� �κθ� ����. ī�޶� ��� �κθ��� ������ �����ش�.
# 3. (51 47) ~ (58 38) �� ��ó�� ���� �� �κθǰ��� ��ȭ�� �Ͼ�� ���� �Լ�.
# 4. �� ������ ������ �������� ����.
#
# Loot:
# ����
#
# Not Implemented Yet
namespace eval EpRoomWorld::EnterDoofuManQuest {
	variable world
	variable pHeroUnit

	proc register {} {
		registerIncidents
	}

	proc registerIncidents {} {
		variable world
		variable pHeroUnit

		set incident	[ EpCreateSequentialIncident 1 ]

		EpAddTriggerToSequence	$incident [ EpCreateUnitPositionTrigger $EpRoomWorld::pHeroUnit 25 91 29 91 0x001 ]
		EpAddActionToSequence	$incident [ EpCreateDialogAction "EpRoomWorld::EnterDoofuManQuest::warmingUp" ]
		
		EpAddTriggerToSequence	$incident [ EpCreateEnemyCountTrigger $EpRoomWorld::world 6 ]
		EpAddActionToSequence	$incident [ EpCreateControllableAction $EpRoomWorld::pHeroUnit 0 ]
		EpAddActionToSequence	$incident [ EpCreateFadeAction out 5000 ]
		EpAddActionToSequence	$incident [ EpCreateCameraAction attach xxx 0 ]
		EpAddActionToSequence	$incident [ EpCreateCameraAction external Camera 0 ]
		EpAddActionToSequence	$incident [ EpCreateFadeAction in 3000 ]		
		EpAddActionToSequence	$incident [ EpCreateScriptAction "EpSetDoAnim [EpGetNode Door] 1" ]
		EpAddActionToSequence	$incident [ EpCreateScriptAction "EpSetDoAnim [EpGetNode TofuMan] 1" ]
		EpAddActionToSequence	$incident [ EpCreateDelayAction	10000 ]
		EpAddActionToSequence	$incident [ EpCreateCameraAction attach xxx 0 ]
		EpAddActionToSequence	$incident [ EpCreateDialogAction "EpRoomWorld::EnterDoofuManQuest::senseOfTufu" ]
		EpAddActionToSequence	$incident [ EpCreateControllableAction $EpRoomWorld::pHeroUnit 1 ]

		EpAddTriggerToSequence	$incident [ EpCreateUnitPositionTrigger $EpRoomWorld::pHeroUnit 51 47 58 38 0x001 ]
		EpAddActionToSequence	$incident [ EpCreateDialogAction "EpRoomWorld::EnterDoofuManQuest::tufu" ]
		
		EpRegisterSequentialIncident $incident
	}

	namespace eval warmingUp {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"

		set dialog [ list\
			$player		"���� ������ ���±�..."\
			$player		"�װ͵� ���� ����!!!"\
			$player		"���̸� �����鼭 ���� �� Ǯ��߰ڴ�."\
		];
	}

	namespace eval tufu {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "��κ�"
		
		set dialog [ list\
			$player		"�ʴ� ������!"\
			$npc		"���� �κθ� ������ ����ϴ� �� ���Ƹ��� �ο��̶��"\
			$player		"�� ���Ƹ��� �Ҽӵ� ����̶�� ��û�Ҹ� �� �ֹ��� ������ �˰� �����ٵ�?"\
			$player		"������ ������ ������ �� Overflow ���ѹ����ھ�!"\
			$npc		"��û�Ҹ� �� ��ȭ�����̶��, A213�̶�� �ź��� ��ҿ��� �ڵ��� ��� �Ѵٰ� �ϴ���"\
			$player		"���� ���ؼ� �����̱�"\
		];	
	}

	namespace eval senseOfTufu {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		
		set dialog [ list\
			$player		"��.. ������ ���� �� ������.."\
			$player		"�� ������ �����߰ڱ�."\
		];	
	}
}