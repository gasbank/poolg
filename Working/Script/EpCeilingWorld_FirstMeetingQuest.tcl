#
# EpCeilingWorld_FirstMeetingQuest.tcl
#
# Goal:
# ���̿��� ù����. ó������ ������ �����Ѵ�.
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
			$npc		"���Ƹ� �濡�� '����' ��� �Ҹ��� ������ ���ƴٴϴ� �� �ϳ�"\
			$player		"���ϴ� �༮�ΰ�?"\
			$npc		"KKK��� �Ҹ��� ����� �׸� ����̶�� �ϴ���...."\
			$player		"�׸��� ���ƴٴҸ��� ���ݾ�"\
			$npc		"����~ ���ʿ� ������ �ѹ� ����ð�"\
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