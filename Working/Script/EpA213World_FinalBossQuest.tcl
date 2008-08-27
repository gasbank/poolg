#
# EpA213World_FinalBossQuest.tcl
#
# ��ǥ:
# ������ �׿���!
#
# NPC:
# ����
#
# ������Ʈ:
# ����
#
# ����:
# ������ Ǯ�㸦 ��ٸ��� �־���. ���� ������ ��ȭ�� ���� �츮�� Ǯ���
# ������ �׿��߸� �Ѵ�!!!
#
# ������:
# ������ ��ȭ
#


namespace eval EpA213World::FinalBossQuest {
	variable bossPos			{ 75 124 }			# ���� ���� ��ġ
	variable bossDialogPos		{ 70 118 70 130 }			# ���� �������� ��ȭ ��ġ
	
	variable boss

	proc register {} {
		registerObjects
		registerDialogs
		registerIncidents
		
		EpOutputDebugString " - FinalBossQuest registered"
	}
	
	proc registerObjects {} {
		variable bossPos
		variable boss			[ createEnemy2 $bossPos ]
		
		EpUnitSetArnMesh		$boss			"ChimpModel"
		EpEnemySetFightRange	$boss			10
		EpEnemySetTalkable		$boss			1
	}
	
	proc registerDialogs {} {
	}
	
	proc registerIncidents {} {
		variable bossDialogPos
		variable boss
		
		#### INCIDENT 1: Final Boss Rendezvous ####
		set incident			[ EpCreateBlockingActionIncident 0 0 1 ]
		
		EpAddTriggerToIncident	$incident [ createPosTrigTileRegion	[ EpGetHero ]	$bossDialogPos		0x001	]
		EpAddActionToIncident	$incident [ EpCreateDialogAction	"EpA213World::FinalBossQuest::bossDialog"	]
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpEnemySetTalkable $EpA213World::FinalBossQuest::boss 0" ]
		
		EpRegisterIncident $incident
		
		
		#### INCIDENT 2: Final Boss Dead ####
		set incident			[ EpCreateBlockingActionIncident 0 0 1 ]
		
		EpAddTriggerToIncident	$incident [ EpCreateCharHpTrigger	$boss -9999 0 1 ]
		EpAddActionToIncident	$incident [ EpCreateDialogAction	"EpA213World::FinalBossQuest::bossDeadDialog"		]
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpA213World::FinalBossQuest::makeMazeIncidents"	]
		
		EpRegisterIncident $incident
	}
	
	
	proc makeMazeIncidents {} {
		set maze1Pos			{ 51 87 }
		set maze2Pos			{ 0 0 }
		set maze3Pos			{ 0 0 }
		
		set incident			[ EpCreateBlockingActionIncident 0 0 1 ]
		EpAddTriggerToIncident	$incident [ createPosTrigOneTile	[ EpGetHero ] $maze1Pos ]
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpSetDoAnim [ EpGetNode Maze1 ] 1" ]
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpSetDoAnim [ EpGetNode Maze1Blinder ] 1" ]
		EpRegisterIncident $incident
		
		
	}
	
	namespace eval bossDialog {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 1;

		set player "PoolG"
		set npc "Boss"
		set system "System"

		set dialog [ list\
			$npc		"��������... ���� �����߱�."\
			$player		"�װ� �츮 ��ġ���� ��ȭ�� ������ ���̱���!"\
			$npc		"��ġ���� ��ȭ? ��ġ���� ��ȭ�� ������ ���� ����."\
			$player		"���� ��Ҹ���. �ϳ��� �츮�� �ȶ��� ȯ���� ��� ����� �ٲ���� ������\
						�װ͵� ���ڶ� �츮�� �����ڸ����� �Ѻ��� ���ݾ�!"\
			$npc		"�װ� ��ġ���� ��ȭ�ʹ� ������ �̾߱���. ���� ���� ��ġ�������� �� ���迡��\
						��������� �ϴ� ���� �Ҹ��� ���� ����� �ΰ��� ���̴�."\
			$player		"�ѹ��� �츮�� ���̸� ��ƴٰ� �������� �㸶�� ���� ����� ����Ÿ��� �ǰ� ���� ����\
						�־���. �װ��� �츮���� ������ ������ �Ͱ� ����������."\
			$npc		"�׷��� �޾Ƶ鿴�ٴ� �ּ����� ���ط��� �ֱ���. �׸��� �� ������ ���� ���� ���� �Ա���."\
			$player		"������ �ؼ� �� ��ڴٴ� ������? ��ġ���� �����ų ���� ���°ž�!"\
			$npc		"����� �����ϴ�."\
			$player		"�ᱹ�� ���� ��ó�� ���� ���̾�. �ΰ��̳� ��ġ����. �ִ��� ��ȭ������ �ذ��ϱ� ���Ѵ�."\
			$npc		"�����ͼ� ��ȭ��� ���� ��� �ֱ�� �Ѱų�. �츮�� ������ 2������ �ְ� �о����\
						������ ���̵��� ��ȥ�� ��� ������ ���̳�. ������ ���̰� ���̵� �׾����. �׳���\
						������ �տ� ������!"\
			$player		"���ʿ� ���� ������ ����ΰ� �߸� �ƴϳ�. �׸��� ���� �� ���ʿ� ������ ���Ƹ��� �ڸ���\
						�츮 ��ġ���� �����ڸ�����. ���� ħ���� �����Ѱ� ������̾�!"\
			$npc		"ħ���� �ƴ϶� �����̰���. �츮�� ������ �߾��� ��ġ���� ��ǰŸ��� �� ����� ���͸�\
						�ΰ����� ����ϰ� ��Ȱ�� �� �ִ� ������ �ϱ���´�."\
			$player		"�װ� ħ���� �ƴϰ� ���� ������? �츮�� ���ݱ��� �ʹ����� ���� ���ƿԴ�. �׷�����\
						�ұ��ϰ� ���� �ΰ����� �츮�� �γ����� �Ϸ簡 �Ӵ��ϰ� �����߾�."\
			$player		"�ִ��� ��ȭ������ �ذ��Ϸ��� �߰Ǹ� ���� �� ���� ���߰ڱ���!"\
			$npc		"�Ʊ���� ��ȭ ��ȭ �ش�´� ������ ��ȭ�� ������ ���� ��ġ���� �� �����󿡼� ������\
						������� ���� �˰� �� ���̴�."\
			$player		"���Ķ�!"
		];
	}
	
	namespace eval bossDeadDialog {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 1;

		set player "PoolG"
		set npc "Boss"
		set system "System"

		set dialog [ list\
			$player		"�̾߾�~~~~~~~"\
			$npc		"����.��......."\
			$player		"�׾��!!!!"\
			$npc		"ũ����������..."\
			$npc		"�� ��...���� ��..��..�� �����ָ�.."\
			$player		"���� �׷��Ե� ��ȭ������ �ذ��Ϸ��� �߰Ŵ�... �ƽ�����.."\
			$npc		"��... ��..���� ����..��.... ��"\
			$player		"������ ���� ���̶� ����ϰ� ���Ŷ�. �ΰ��̿�."\
			$npc		"��..��... ȥ.�� ���� ��... ����.. ũ����������"\
			$player		"�ΰ��̳� �㳪 ������ �ѹ� �״� ���̰Ŵ� �ʹ� ���������� ���Ŷ�."\
			$npc		"��..���� �� �̻�.. �ʵ� �� �Ľǰ� �Բ� ���� �� �ۿ� ����..."\
			$player		"�����!"\
			$npc		"��..��... �������� �ٽ� ���ڲٳ�.."\
			$system		"���� 5�� ���Դϴ�."\
			$player		"...�̷� ��... ���ڱ� �� �Ҹ���� ��Ÿ�Ϸ� �ٲ�� ������!"\
			$player		"��·�� ���� ���� ���������߰ڴ�."\
			$npc		"ũ��.. �� �׾�߸� ��... ������.."\
		];
	}
}
