#
# EpCeilingWorld_FirstMeetingQuest.tcl
#
# Goal:
# 괭이와의 첫만남. 처음으로 전투를 경험한다.
#
# NPCs:
# Gloop weakCat
#
# Objects:
#
# Description:
#
# Detail:
#
# Loot:
#
namespace eval EpCeilingWorld::FirstMeetingQuest {
	variable weakCat
	variable pHeroUnit
	variable npcGloop

	proc register {} {
		registerObjects
		registerIncident1
	}

	proc registerObjects {} {
		variable weakCat
		variable pHeroUnit
		variable npcGloop

		set weakCat					[ createEnemy 42 75 ];
		EpUnitSetColor				$weakCat 0 0 0 
		EpCharacterSetStat			$weakCat 2 2 2 2 2 2
		EpCharacterSetCurHp			$weakCat 23
		EpEnemySetTalkable			$weakCat 1
		EpUnitSetArnMesh			$weakCat "GwengYiModel"

		set npcGloop				[ createEnemy 30 82 ];
		EpUnitSetColor				$npcGloop 0 255 255
		EpCharacterSetStat			$npcGloop 3 1 4 1 1 1
		EpCharacterSetCurHp			$npcGloop -1
		EpEnemySetTalkable			$npcGloop 1
		EpUnitSetArnMesh			$npcGloop "PoolGModel"
		EpUnitSetName				$npcGloop "Gloop"
		EpUnitSetNameVisible		$npcGloop 1

	}

	proc registerIncident1 {} {
		variable weakCat
		variable pHeroUnit
		variable npcGloop

		set incident	[ EpCreateSequentialIncident 1 ]

		EpAddTriggerToSequence	$incident	[ EpCreateUnitPositionWithTraceTrigger $EpCeilingWorld::pHeroUnit $npcGloop 0x001 ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::FirstMeetingQuest::GlooPDialog" ]
		
		EpAddTriggerToSequence	$incident	[ EpCreateUnitPositionWithTraceTrigger $EpCeilingWorld::pHeroUnit $weakCat 0x001 ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::FirstMeetingQuest::weakCatDialog" ]
		EpAddActionToSequence	$incident	[ EpCreateScriptAction "EpEnemySetTalkable $weakCat 0" ]

		EpAddTriggerToSequence	$incident	[ EpRemoveFlagTrigger $weakCat ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::FirstMeetingQuest::catDeadDialog" ]

		EpAddTriggerToSequence	$incident	[ EpCreateUnitPositionWithTraceTrigger $EpCeilingWorld::pHeroUnit $npcGloop 0x001 ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::FirstMeetingQuest::GlooPDialog2" ]
		
		EpRegisterIncident $incident
	}

	# Not Used
	proc registerIncident2 {} {
		variable pHeroUnit
		variable npcGloop

		set incident	 [ EpCreateSequentialIncident -1 ]

		EpAddTriggerToSequence	$incident [ EpCreateUnitPositionWithTraceTrigger $EpCeilingWorld::pHeroUnit $npcGloop 0x010 ]
		EpAddActionToSequence	$incident [ EpCreateScriptAction "#" ]
		EpAddTriggerToSequence	$incident [ EpCreateUnitPositionWithTraceTrigger $EpCeilingWorld::pHeroUnit $npcGloop 0x001 ]
		EpAddActionToSequence	$incident [ EpCreateDialogAction "EpCeilingWorld::FirstMeetingQuest::GlooPDialog" ]

		EpRegisterIncident $incident

		## --------------------------------------------
		## Do not register a incident planned
		## to be invoked by EpCreateStartIncidentAndWaitAction!!!
		## --------------------------------------------
		#EpOutputDebugString " - Incident count: $incCount\n"

		#EpOutputDebugString " - Quest2 Incident returned $incident\n"
		#return $incident
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
			$npc		"저기~ 동쪽에 있으니 한번 물어보시게"\
		];	
	}

	namespace eval rGlooPDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GlooP"
	
		set dialog [ list\
			$npc		"물어보고 오라니깐?"\
			$player		"가서 싸우는게 아니라?"\
			$npc		"들켰나!"\
		];	
	}

	namespace eval GlooPDialog2 {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GlooP"
	
		set dialog [ list\
			$npc		"잘했네"\
			$player		"아 예"\
		];	
	}

	
	namespace eval weakCatDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set enemy "GwengYi"
	
		set dialog [ list\
			$enemy		"여긴 어디지? 난 누구?"\
			$player		"..... 뭐야 이건"\
			$enemy		"우오!!!!!!!!!!!!!!!"\
			$player		"뭐, 뭐냐!"\
		];	
	}

	namespace eval catDeadDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
	
		set dialog [ list\
			$player		"적장, 물리쳤다!"\
		];	
	}
}