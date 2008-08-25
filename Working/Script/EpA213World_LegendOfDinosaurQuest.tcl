namespace eval EpA213World {
	# -----------------------------------------------------------------------------------------
	# ���� : ������ ����
	# ���� : A213�� ������� ��� ���� �㿴��. �� ����� A213�� ���Դٰ� ���翵�� ������
	# ������ �ڵ� ���Ǹ��� ��ο� ���� ��� �Ұ� A213�� ��ȸ�ϴ� �������� �ǰ� ���Ҵ�.
	# ������ ���� �ڵ� ���Ǹ��� ��ο� ���� �� ���� ���� �� ����(43, 54)�� ���ΰ����� "Big Rigs Limited
	# Special Package"�� ã�ƿ��� �ڽ��� �ٽ� ��� ���Ƽ� �ִٰ� ���ΰ����� ��Ź�� �Ѵ�.
	# �׸��� �� �������� A213 ������ ������ �� ��(25, 45)�� ������ �ִ�.
	# ���ΰ��� "Big Rigs..."�� �������鼭 �ٸ� ����� ����(12, 56)�� �޴´�.
	proc registerUnitsLegendOfDinosaur {} {
		set pNPC_GoodDinosaur		[ createEnemy 43 54 ]
		EpUnitSetColor				$pNPC_GoodDinosaur 255 128 0
		EpUnitSetArnMesh			$pNPC_GoodDinosaur "GwengYiModel"
		EpEnemySetTalkable			$pNPC_GoodDinosaur 1
		EpUnitSetName				$pNPC_GoodDinosaur "Gentle Dinosaur"
		EpUnitSetNameVisible		$pNPC_GoodDinosaur 1

		set pEnemy_EvilRat			[ createEnemy 25 45 ]
		EpUnitSetColor				$pEnemy_EvilRat	255 0 0
		EpUnitSetArnMesh			$pEnemy_EvilRat "PoolGModel"
		EpEnemySetTalkable			$pEnemy_EvilRat 1
		EpUnitSetName				$pEnemy_EvilRat "Evil Rat"
		EpUnitSetNameVisible		$pEnemy_EvilRat	1
	}

	proc registerIncident_LegendOfDinosaur {} {
		variable pHeroUnit
		global	pNPC_GoodDinosaur pEnemy_EvilRat

		set seqIncident		[ EpCreateSequentialIncident 1 ]

		set triggerA0		[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $pNPC_GoodDinosaur 0x001 ]
		set actionA0		[ EpCreateDialogAction "EpA213World::Dialog_LegendOfDinosaur_GoodDinosaur0" ]

		set triggerB0		[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $pEnemy_EvilRat 0x001 ]
		set actionB0		[ EpCreateDialogAction "EpA213World::Dialog_LegendOfDinosaur_EvilRat" ]
		set actionB1		[ EpCreateScriptAction "EpEnemySetTalkable $pEnemy_EvilRat 0" ]

		set triggerC0		[ EpCreateUnitPositionTrigger $pHeroUnit 10 54 14 53 0x001 ]
		set actionC1		[ EpCreateDialogAction "EpA213World::Dialog_LegendOfDinosaur_BigRigsAssault" ]
		set actionC2		[ EpCreateScriptAction "createEnemy 11 55" ]
		set actionC3		[ EpCreateScriptAction "createEnemy 13 55" ]

		set triggerD0		[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $pNPC_GoodDinosaur 0x001 ]
		set actionD0		[ EpCreateDialogAction "EpA213World::Dialog_LegendOfDinosaur_GoodDinosaur1" ]
		set actionD1		[ EpCreateScriptAction "EpUnitSetRemoveFlag $pNPC_GoodDInosaur 1" ]
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

		EpSequentialIncidentSetName	$seqIncident "LegendOfDinsaur sequential incident"

		set incCount		[ EpRegisterSequentialIncident $seqIncident ]
		
		EpOutputDebugString " - LegendOfDinsaur Incident returned $seqIncident\n"
		return $seqIncident
	}

	namespace eval Dialog_LegendOfDinosaur_GoodDinosaur0 {} {
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

	namespace eval Dialog_LegendOfDinosaur_EvilRat {} {
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

	namespace eval Dialog_LegendOfDinosaur_BigRigsAssault {} {
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

	namespace eval Dialog_LegendOfDinosaur_GoodDinosaur1 {} {
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