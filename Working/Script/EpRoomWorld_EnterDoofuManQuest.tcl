# Not Implemented Yet
#
# EpRoomWorld_EnterDoofuManQuest.tcl
#
# Goal:
# 적당한 수의 괭이를 처치하고 두부맨으로부터 정보 입수
#
# NPCs:
# 없음
#
# Objects:
# 없음
#
# Description:
# 플레이어는 괭이를 잡으면서 수련 해야한다는 사실을 직감한다. 적당한 수의 괭이를
# 처치하면 우연치 않게 두부가 방문을 열고 들어와 대화가 시작된다. 대화를 통해서
# 플레이어는 Dark Lord Choi가 A213에 있다는 정보를 입수하고 문 밖으로 나간다.
#
# Detail:
# 1. (25, 91) - (29, 91) 이 길목을 지나면서 풀쥐는 괭이를 잡으면서 몸을 풀어야 겠다고 생각한다.
# 2. 괭이가 4마리 남았을 때 두부맨 등장. 카메라가 잠시 두부맨의 등장을 보여준다.
# 3. 문 근처에 갔을 때 두부맨과의 대화가 일어나며 정보 입수.
# 4. 문 밖으로 나가며 스테이지 종료.
#
# Loot:
# 없음
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