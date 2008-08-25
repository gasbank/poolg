namespace eval EpA213World {
# -----------------------------------------------------------------------------------------
	# ���� : ���� ���� ������ ������
	# ���� : ���� ����� ��ȭ�� ���� A213�� ���� ������ ��� �ǰ�, �����⸦ ��ġ��� A213�� ����
	# ���� ���� �ʼ� ������ ���� �κθ� ��´�.
	# registerIncident_TalkWithOldRat
	# registerIncident_pEnemy_OldCat1 
	proc registerIncident_TalkWithOldRat {} {
		variable pHeroUnit
		global pNPC_OldRat pEnemy_OldCat10 pEnemy_OldCat11
		set animObjects [ list Blocking1 Blocking2 Blocking3 GateRight GateLeft GateCamera ]

		set sqIncident		[ EpCreateSequentialIncident 1 ]

		set trigger0		[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $pNPC_OldRat 0x001 ]
		set action0			[ EpCreateDialogAction "EpA213World::NPC_OldRatDialog" ]

		set action1			[ EpCreateScriptAction "EpEnemySetTalkable pEnemy_OldCat1 1" ]

		set trigger1		[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $pEnemy_OldCat10 0x001 ]
		set action1			[ EpCreateDialogAction "EpA213World::NPC_OldCat10Dialog" ]
		#set action2			[ EpCreateScriptAction "Incident_GateOpen" ]

		EpAddTriggerToSequence		$sqIncident $trigger0
		EpAddActionToSequence		$sqIncident	$action0
		EpAddTriggerToSequence		$sqIncident	$trigger1
		EpAddActionToSequence		$sqIncident	$action1
		#EpAddActionToSequence		$sqIncident	$action2

		# �� ���� ---------------------------------------------------------------------------------
		set actions			[ EpCreateControllableAction	$pHeroUnit 0 ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Blocking1  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Blocking2  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Blocking3  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode GateRight  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode GateLeft   ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode GateCamera ] 1" ]
		lappend actions		[ EpCreateCameraAction			external GateCamera 0 ]
		lappend actions		[ EpCreateDelayAction			10000 ]
		lappend actions		[ EpCreateCameraAction			attach GateCamera 1500 ]
		lappend actions		[ EpCreateControllableAction	$pHeroUnit 1 ]

		foreach act $actions {
			EpAddActionToSequence $sqIncident $act
		}
		# ------------------------------------------------------------------------------------------

		set action					[ EpCreateScriptAction	"EpEnemySetTalkable $pEnemy_OldCat11 0" ]
		EpAddActionToSequence		$sqIncident	$action
		set action					[ EpCreateScriptAction	"EpUnitSetRemoveFlag $pEnemy_OldCat10 1" ]
		EpAddActionToSequence		$sqIncident $action
		
		EpSequentialIncidentSetName	$sqIncident "TalkWithRat incident"
		
		set incCount		[ EpRegisterSequentialIncident $sqIncident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncident_pEnemy_OldCat1 {} {
		variable pHeroUnit
		global pNPC_OldRat pEnemy_OldCat1 pEnemy_OldCat11

		set trigger0		[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $pNPC_OldRat 0x001 ]
		set trigger1		[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $pEnemy_OldCat1 0x001 ]
		set action0		[ EpCreateScriptAction "#" ]
		set action1		[ EpCreateDialogAction "EpA213World::NPC_OldCat1Dialog" ]
		set action2		[ EpCreateScriptAction "EpEnemySetTalkable $pEnemy_OldCat1 0" ]
				
		set seqIncident	[ EpCreateSequentialIncident 1 ]
		EpAddTriggerToSequence $seqIncident $trigger0
		EpAddActionToSequence $seqIncident $action0
		EpAddTriggerToSequence $seqIncident $trigger1
		EpAddActionToSequence $seqIncident $action1
		EpAddActionToSequence $seqIncident $action2

		set action					[ EpCreateScriptAction	"EpEnemySetTalkable $pEnemy_OldCat11 0" ]
		EpAddActionToSequence		$seqIncident	$action

		EpSequentialIncidentSetName	$seqIncident "pEnemy_OldCat1 sequential incident"

		set incCount		[ EpRegisterSequentialIncident $seqIncident ]
		
		EpOutputDebugString " - pEnemy_OldCat1 Incident returned $seqIncident\n"
		return $seqIncident
	}

		namespace eval NPC_OldRatDialog {} {
	
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

	namespace eval NPC_OldCat10Dialog {} {
	
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

	namespace eval NPC_OldCat1Dialog {} {
		
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