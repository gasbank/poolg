#
# EpA213World_OldRatandGatekeeperQuest.tcl
#
# ���� : ���� ���� ������ ������
#
# ���� : ���� ����� ��ȭ�� ���� A213�� ���� ������ ��� �ǰ�, �����⸦ ��ġ��� A213�� ����
# ���� ���� �ʼ� ������ ���� �κθ� ��´�.
#

namespace eval EpA213World::OldRatandGatekeeperQuest {
	global pOldRat pOldCat1 pOldCat10 pOldCat11\
			pGoodDinosaur pEvilRat

	proc register {} {
		registerUnitsOldRatandGatekeeper
		registerIncident_TalkWithOldRat
		registerIncident_pOldCat1
	}

	proc registerUnitsOldRatandGatekeeper {} {
		global pOldRat pOldCat1 pOldCat10 pOldCat11\
		pGoodDinosaur pEvilRat

		set pOldRat				[ createEnemy 4 59 ]
		EpUnitSetArnMesh			$pOldRat "PoolGModel"
		EpUnitSetColor				$pOldRat 128 128 128
		EpCharacterSetStat			$pOldRat 4 1 3 1 1 1
		EpCharacterSetCurHp			$pOldRat -1
		EpEnemySetTalkable			$pOldRat 1
		EpUnitSetName				$pOldRat "An Old Rat..."
		EpUnitSetNameVisible		$pOldRat 1

		set	pOldCat1			[ createEnemy 21 65 ]
		EpUnitSetArnMesh			$pOldCat1 "GwengYiModel"
		EpUnitSetColor				$pOldCat1 128 128 128
		EpCharacterSetStat			$pOldCat1 10 10 10 10 10 10
		EpCharacterSetCurHp			$pOldCat1 100
		EpEnemySetTalkable			$pOldCat1 1
		EpUnitSetName				$pOldCat1 "No.1 Gatekeeper"
		EpUnitSetNameVisible		$pOldCat1 1

		set	pOldCat10			[ createEnemy 21 70 ]
		EpUnitSetArnMesh			$pOldCat10 "GwengYiModel"
		EpUnitSetColor				$pOldCat10 128 128 128
		EpCharacterSetStat			$pOldCat10 10 10 10 10 10 10
		EpCharacterSetCurHp			$pOldCat10 100
		EpEnemySetTalkable			$pOldCat10 1
		EpUnitSetName				$pOldCat10 "No.10 Gatekeeper"
		EpUnitSetNameVisible		$pOldCat10 1

		set	pOldCat11			[ createEnemy 21 60 ]
		EpUnitSetArnMesh			$pOldCat11 "GwengYiModel"
		EpEnemySetTalkable			$pOldCat11 1
		EpUnitSetName				$pOldCat11 "No.11 Gatekeeper"
		EpUnitSetNameVisible		$pOldCat11 1
	}

	proc registerIncident_TalkWithOldRat {} {
		variable pHeroUnit
		global pOldRat pOldCat10 pOldCat11
		set animObjects [ list Blocking1 Blocking2 Blocking3 GateRight GateLeft GateCamera ]

		set sqIncident		[ EpCreateSequentialIncident 1 ]

		set trigger0		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pOldRat 0x001 ]
		set action0			[ EpCreateDialogAction "EpA213World::OldRatandGatekeeperQuest::oldRatDialog" ]

		set trigger1		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pOldCat10 0x001 ]
		set action1			[ EpCreateDialogAction "EpA213World::OldRatandGatekeeperQuest::oldCat10Dialog" ]

		EpAddTriggerToSequence		$sqIncident $trigger0
		EpAddActionToSequence		$sqIncident	$action0
		EpAddTriggerToSequence		$sqIncident	$trigger1
		EpAddActionToSequence		$sqIncident	$action1

		# �� ���� ---------------------------------------------------------------------------------
		set actions			[ EpCreateControllableAction	$EpA213World::pHeroUnit 0								]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Blocking1  ] 1"	]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Blocking2  ] 1"	]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Blocking3  ] 1"	]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode GateRight  ] 1"	]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode GateLeft   ] 1"	]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode GateCamera ] 1"	]
		lappend actions		[ EpCreateCameraAction			external GateCamera 0						]
		#lappend actions		[ EpCreateScriptAction			"EpCameraSetShake 1"						]
		lappend actions		[ EpCreateDelayAction			10000										]
		lappend actions		[ EpCreateCameraAction			attach xxx 1500								]
		#lappend actions		[ EpCreateScriptAction			"EpCameraSetShake 0"						]
		lappend actions		[ EpCreateControllableAction	$EpA213World::pHeroUnit 1								]

		foreach act $actions {
			EpAddActionToSequence $sqIncident $act
		}
		# ------------------------------------------------------------------------------------------

		set action					[ EpCreateScriptAction	"EpEnemySetTalkable $pOldCat11 0" ]
		EpAddActionToSequence		$sqIncident	$action
		set action					[ EpCreateScriptAction	"EpUnitSetRemoveFlag $pOldCat10 1" ]
		EpAddActionToSequence		$sqIncident $action
		
		EpIncidentSetName	$sqIncident "TalkWithRat incident"
		
		set incCount		[ EpRegisterIncident $sqIncident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncident_pOldCat1 {} {
		variable pHeroUnit
		global pOldRat pOldCat1 pOldCat11

		set trigger0		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pOldRat 0x001 ]
		set trigger1		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pOldCat1 0x001 ]
		set action0		[ EpCreateScriptAction "#" ]
		set action1		[ EpCreateDialogAction "EpA213World::OldRatandGatekeeperQuest::oldCat1Dialog" ]
		set action2		[ EpCreateScriptAction "EpEnemySetTalkable $pOldCat1 0" ]
				
		set seqIncident	[ EpCreateSequentialIncident 1 ]
		EpAddTriggerToSequence $seqIncident $trigger0
		EpAddActionToSequence $seqIncident $action0
		EpAddTriggerToSequence $seqIncident $trigger1
		EpAddActionToSequence $seqIncident $action1
		EpAddActionToSequence $seqIncident $action2

		set action					[ EpCreateScriptAction	"EpEnemySetTalkable $pOldCat11 0" ]
		EpAddActionToSequence		$seqIncident	$action

		EpIncidentSetName	$seqIncident "pOldCat1 sequential incident"

		set incCount		[ EpRegisterIncident $seqIncident ]
		
		EpOutputDebugString " - pOldCat1 Incident returned $seqIncident\n"
		return $seqIncident
	}

	namespace eval oldRatDialog {} {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "An Old Rat..."
		set	system "System"
	
		set dialog [ list\
			$npc		"�ڳ׵� Dark Lord Choi�� ������ �Դ°�?"\
			$player		"....."\
			$npc		"�ڳ��� ��� ��ο� �ڵ� ���Ǹ��� �����ϰ� �� �� �־���.."\
			$npc		"�� �׿��� �����Ϸ� �ϴ°հ�?"\
			$player		"�����ϱ� ���ؼ� �Դϴ�."\
			$npc		"�׷���..."\
			$npc		"���� �ڳ�ó�� ���� ���� �־���."\
			$npc		"A213���� ���°� �ù��� ǳ���ϴٴ� �ҹ��� �־���."\
			$npc		"������� ��� ������ ������, ���⿡ ��������. �ڳ�ó�� ���̾�..."\
			$npc		"�׸��� �׸� ������..."\
			$npc		"�״� ���� ���߾�! Ctr-C, Ctr-V ���ݿ� �״� �� �ϳ� �������� �ʾҾ�!"\
			$npc		"������� ��� �׾���, ���� ����ϰ� ȥ�� ��� ���Դٳ�."\
			$npc		"�׸��� �� ���ķ� ���� ��� ���⿡�� ��� ����..."\
			$player		"�� ���⼭ ��ʴϱ�?"\
			$npc		"�ڳװ��� ģ���� ���ؼ��ϼ�"\
			$npc		"���� ���� �׳� ���ķ�, �� ���� �Ѿ�� ��� ���ƿ� ��� �����ٳ�."\
			$npc		"�ڳ׵� ��ó�� ��� ���� ���� ����, ���ư��Գ�."\
			$player		"���� �̹� ��û�ҷ� ��� ���� �Ҿ����ϴ�."\
			$player		"�ݵ�� �� ���� ������, �׿��� �����ؾ߸� �մϴ�."\
			$npc		"�ҿ��� ������.."\
			$npc		"���� ���ڴٸ�, �� ��Ź �ϳ��� ����ְڳ�?"\
			$player		"�����Դϱ�?"\
			$npc		"���� �������뿡�� �Դ� ���� �κ� ĵ�� �ϳ� �ֿ���"\
			$npc		"���� �ϵ����� 2�� ������� �̰��� ������ �԰� ������, ���� �İ� ���� �����̱Ⱑ ������."\
			$npc		"�ڳװ� �̰��� �� ������ �ְ�."\
			$player		"��"\
			$system		"���� �κθ�(��) ������ϴ�."\
		];	
	}

	namespace eval oldCat10Dialog {} {
	
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "Old Cat(Gatekeeper)"
		set	system "System"
	
		set dialog [ list\
			$npc		"�κ� ������ ���±�."\
			$player		"���� �帱���?"\
			$npc		"�� �׷��� ���ھ�."\
			$player		"�����."\
			$npc		"(�ȳ�...)"\
			$npc		"(����...)"\
			$npc		"(��ƿ��...)"\
			$npc		"(�ܲ�!)"\
			$npc		"��!!!!!!!!!!!"\
			$npc		"�����ݾ�!!!!"\
			$npc		"(�н�)"\
			$system		"����� ������ ������κ��� ���踦 �� ���ۿ� ���� ���� ���Ҵ�."\
		];	
	}

	namespace eval oldCat1Dialog {} {
		
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "Old Cat(Gatekeeper)"
		set	system "System"
	
		set dialog [ list\
			$npc		"�̰� ���� ������?"\
			$player		"�κ�"\
			$npc		"�����ϴ�! ���� ���� �Ⱦ��ϴ� �κθ� �������ٴ�!"\
		];	
	}
}