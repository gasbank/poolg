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
	variable bossPos			{ 75 124 }					# ���� ���� ��ġ
	variable bossDialogPos		{ 70 118 70 130 }			# ���� �������� ��ȭ ��ġ
	variable exitPos			{ 8 51 17 51 }				# �ⱸ ��ġ
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
		EpCharacterSetStat		$boss			7 5 7 5 5 5 ;# Health Will Coding Defence Sense Immunity
		EpCharacterSetCurHp		$pHeroUnit		75
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
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpA213World::FinalBossQuest::endingIncidents"	]
		EpRegisterIncident		$incident
	}
	
	
	proc makeMazeIncidents {} {
		set mazePos			{ 51 87 }
		
		set incident			[ EpCreateBlockingActionIncident 0 0 1 ]
		EpAddTriggerToIncident	$incident [ createPosTrigOneTile	[ EpGetHero ] $mazePos						]
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpA213World::FinalBossQuest::doMazeAnim"	]
		EpAddActionToIncident	$incident [ EpCreateCameraAction	external MazeCamera 500						]
		EpAddActionToIncident	$incident [ EpCreateDelayAction		13000										]
		EpAddActionToIncident	$incident [ EpCreateCameraAction	attach xxx 0								]
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpCameraSetShake 1"						]
		EpRegisterIncident		$incident
	}

	proc doMazeAnim {} {
		EpSetDoAnim [ EpGetNode Maze1			]	1
		EpSetDoAnim [ EpGetNode PopupChair		]	1
		EpSetDoAnim [ EpGetNode PopupDesk		]	1
		EpSetDoAnim [ EpGetNode PopupServer1	]	1
		EpSetDoAnim [ EpGetNode PopupServer2	]	1
		EpSetDoAnim [ EpGetNode MazeCamera		]	1
		
	}
	
	
	proc endingIncidents {} {
		variable exitPos
		
		set incident			[ EpCreateBlockingActionIncident 0 0 1 ]
		EpAddTriggerToIncident	$incident [ createPosTrigTileRegion	[ EpGetHero ] $exitPos							]
		EpAddActionToIncident	$incident [ EpCreateDialogAction	"EpA213World::FinalBossQuest::exitA213Dialog"	]
		EpAddActionToIncident	$incident [ EpCreateFadeAction		"out" 3500										]
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpCameraSetShake 0"							]
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpChangeWorld EpCeilingWorld"					]
		EpRegisterIncident		$incident
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
	
	namespace eval exitA213Dialog {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 1;

		set player "PoolG"

		set dialog [ list\
			$player		"���� ��� ���� ���̱���..."\
		];
	}
}
