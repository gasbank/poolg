#
# EpA213World_LegendOfDinosaurQuest.tcl
#
# ���� : ������ ����
#
# ���� : A213�� ������� ��� ���� �㿴��. �� ����� A213�� ���Դٰ� ���翵�� ������
# ������ �ڵ� ���Ǹ��� ��ο� ���� ��� �Ұ� A213�� ��ȸ�ϴ� �������� �ǰ� ���Ҵ�.
# ������ ���� �ڵ� ���Ǹ��� ��ο� ���� �� ���� ���� �� ����(43, 54)�� ���ΰ����� "Big Rigs Limited
# Special Package"�� ã�ƿ��� �ڽ��� �ٽ� ��� ���Ƽ� �ִٰ� ���ΰ����� ��Ź�� �Ѵ�.
# �׸��� �� �������� A213 ������ ������ �� ��(25, 45)�� ������ �ִ�.
# ���ΰ��� "Big Rigs..."�� �������鼭 �ٸ� ����� ����(12, 56)�� �޴´�.
#

namespace eval EpA213World::LegendOfDinosaurQuest {
	global pOldRat pOldCat1 pOldCat10 pOldCat11\
			pGoodDinosaur pEvilRat

	proc register {} {
		registerUnitsLegendOfDinosaur		
		registerIncident_LegendOfDinosaur
	}

	proc registerUnitsLegendOfDinosaur {} {
		global pOldRat pOldCat1 pOldCat10 pOldCat11\
		pGoodDinosaur pEvilRat

		set pGoodDinosaur		[ createEnemy 44 74 ]
		EpUnitSetColor				$pGoodDinosaur 255 128 0
		EpUnitSetArnMesh			$pGoodDinosaur "DinoModel"
		EpEnemySetTalkable			$pGoodDinosaur 1
		EpUnitSetName				$pGoodDinosaur "Gentle Dinosaur"
		EpUnitSetNameVisible		$pGoodDinosaur 1

		set pEvilRat			[ createEnemy 25 45 ]
		EpUnitSetColor				$pEvilRat	255 0 0
		EpUnitSetArnMesh			$pEvilRat "PoolGModel"
		EpEnemySetTalkable			$pEvilRat 1
		EpUnitSetName				$pEvilRat "Evil Rat"
		EpUnitSetNameVisible		$pEvilRat	1
	}

	proc registerIncident_LegendOfDinosaur {} {
		variable pHeroUnit
		global	pGoodDinosaur pEvilRat

		set seqIncident		[ EpCreateSequentialIncident 1 ]

		set triggerA0		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pGoodDinosaur 0x001 ]
		set actionA0		[ EpCreateDialogAction "EpA213World::LegendOfDinosaurQuest::goodDinosaurDialog0" ]

		set triggerB0		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pEvilRat 0x001 ]
		set actionB0		[ EpCreateDialogAction "EpA213World::LegendOfDinosaurQuest::evilRatDialog" ]
		set actionB1		[ EpCreateScriptAction "EpEnemySetTalkable $pEvilRat 0" ]

		set triggerC0		[ EpCreateUnitPositionTrigger $EpA213World::pHeroUnit 10 54 14 53 0x001 ]
		set actionC1		[ EpCreateDialogAction "EpA213World::LegendOfDinosaurQuest::bigRigsAssaultDialog" ]
		set actionC2		[ EpCreateScriptAction "createEnemy 11 55" ]
		set actionC3		[ EpCreateScriptAction "createEnemy 13 55" ]

		set triggerD0		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pGoodDinosaur 0x001 ]
		set actionD0		[ EpCreateDialogAction "EpA213World::LegendOfDinosaurQuest::goodDinosaurDialog1" ]
		set actionD1		[ EpCreateScriptAction "EpUnitSetRemoveFlag $pGoodDinosaur 1" ]
		# TODO: ���� ������ ������ų� ��� ���ϰų� �ؾ� �Ѵ�.

		EpAddTriggerToSequence	$seqIncident $triggerA0
		EpAddActionToSequence	$seqIncident $actionA0

		EpAddTriggerToSequence	$seqIncident $triggerB0
		EpAddActionToSequence	$seqIncident $actionB0
		EpAddActionToSequence	$seqIncident $actionB1

		EpAddTriggerToSequence	$seqIncident $triggerC0
		EpAddActionToSequence	$seqIncident $actionC1
		EpAddActionToSequence	$seqIncident $actionC2
		EpAddActionToSequence	$seqIncident $actionC3

		EpAddTriggerToSequence	$seqIncident $triggerD0
		EpAddActionToSequence	$seqIncident $actionD0
		EpAddActionToSequence	$seqIncident $actionD1

		EpIncidentSetName	$seqIncident "LegendOfDinsaur sequential incident"

		set incCount		[ EpRegisterSequentialIncident $seqIncident ]
		
		EpOutputDebugString " - LegendOfDinsaur Incident returned $seqIncident\n"
		return $seqIncident
	}

	namespace eval goodDinosaurDialog0 {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "Gentle Dinosaur"
		set system "System"

		set dialog [ list\
			$npc		"�̺���"\
			$player		"������ ���� �ϴٴ�!"\
			$npc		"�����ּ���."\
			$player		"������ ���͵帱���?"\
			$npc		"���� ��� ������� ���� �ڵ� ���Ǹ��� ��ο� ���� �游�� ��Ű� ���� ����̾���"\
			$npc		"������ A213�� ���� Dark Lord Choi�� ���ں��� ������ ���� �������� �ڵ� ���Ǹ��� ��� �Ұ�"\
			$npc		"�̷��� ������ �Ǿ� �̰��� ��ȸ�ϰ� �� ���̿���..."\
			$player		"�׷���."\
			$npc		"���� �� �̼��� �Ұ� ��ȸ�ϴ� ������ �� �ſ���."\
			$npc		"������ �װ� �ִٸ�..."\
			$player		"�װŶ��, Big Rigs Limited Special Package�� ���ϴ� �ǰ�?"\
			$npc		"�ٷ� �װſ���!"\
			$npc		"A213�� ������ �������� ���� ���� ���Ͱ� �־��."\
			$npc		"�ű⿡ ���� Big Rigs Limited Special Package�� ���� ���Ҿ��."\
			$player		"�˰ڳ�."\
		];
	}

	namespace eval evilRatDialog {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "Evil Rat"
		set system "System"

		set dialog [ list\
			$player		"�� Big Rigs Limited Special Package�� ��� ���ǰ�?"\
			$npc		"My pressure..."\
			$player		"�� ��������"\
			$npc		"�ȵ�! �̰� ���ž�!!"\
		];
	}

	namespace eval bigRigsAssaultDialog {} {
	set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "Assassin Rat"
		set system "System"

		set dialog [ list\
			$npc		"Big Rigs Limited Special Package�� ������ �ֱ�."\
			$player		"������!"\
			$npc		"�׾���߰ڴ�."\
		];
	}

	namespace eval goodDinosaurDialog1 {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "Gentle Dinosaur"
		set system "System"

		set dialog [ list\
			$npc		"�������̱���!"\
			$npc		"���� ������. ���� ���� �ٽ� ��� ���ư� �� �ְ� �Ǿ����."\
			$player		"��������"\
		];
	}
}
