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
# 1. (25, 91) ~ (29, 91) 이 길목을 지나면서 풀쥐는 괭이를 잡으면서 몸을 풀어야 겠다고 생각한다.
# 2. 괭이가 4마리 남았을 때 두부맨 등장. 카메라가 잠시 두부맨의 등장을 보여준다.
# 3. (51 47) ~ (58 38) 문 근처에 갔을 때 두부맨과의 대화가 일어나며 정보 입수.
# 4. 문 밖으로 나가며 스테이지 종료.
#
# Loot:
# 없음
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
			$player		"괭이 냄세가 나는군..."\
			$player		"그것도 아주 많이!!!"\
			$player		"괭이를 잡으면서 몸을 좀 풀어야겠다."\
		];
	}

	namespace eval tufu {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "김두부"
		
		set dialog [ list\
			$player		"너는 누구냐!"\
			$npc		"나는 두부를 지극히 사랑하는 이 동아리의 부원이라네"\
			$player		"이 동아리에 소속된 사람이라면 대청소를 한 주범이 누군지 알고 있을텐데?"\
			$player		"순순히 말하지 않으면 널 Overflow 시켜버리겠어!"\
			$npc		"대청소를 한 미화부장이라면, A213이라는 신비의 장소에서 코딩을 즐겨 한다고 하더군"\
			$player		"말이 통해서 다행이군"\
		];	
	}

	namespace eval senseOfTufu {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		
		set dialog [ list\
			$player		"음.. 누군가 들어온 것 같은데.."\
			$player		"문 쪽으로 가봐야겠군."\
		];	
	}
}