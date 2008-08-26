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
# 1. (25, 91) - (29, 91) �� ����� �����鼭 Ǯ��� ���̸� �����鼭 ���� Ǯ��� �ڴٰ� �����Ѵ�.
# 2. ���̰� 4���� ������ �� �κθ� ����. ī�޶� ��� �κθ��� ������ �����ش�.
# 3. �� ��ó�� ���� �� �κθǰ��� ��ȭ�� �Ͼ�� ���� �Լ�.
# 4. �� ������ ������ �������� ����.
#
# Loot:
# ����
#
# Not Implemented Yet
namespace eval EpRoomWorld::EnterDoofuMan {
	proc registerIncidents {} {
		variable world
		variable pHeroUnit
		variable pTestEnemyUnit

		set incident	[ EpCreateSequentialIncident $trigger 0 1 ]

		EpAddTriggerToSequence $incident [ EpCreateUnitPositionTrigger $

		set trigger		[ EpCreateUnitPositionTrigger $TestStObject 50 39 -1 -1 0x101 ]
		set action1		[ EpCreateStartAnimationAction [ EpGetNode Door ] ]
		set action2		[ EpCreateStartAnimationAction [ EpGetNode TofuMan ] ]
		

		EpAddActionToIncident $incident $action1
		EpAddActionToIncident $incident $action2
		
		EpIncidentSetName	$incident "Switch incident"
		EpIncidentSetControlDuringAction $incident 1
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
}