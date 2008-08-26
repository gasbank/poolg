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

proc createPosTrigTileRegion { unit tileRegion { flags 0x001 } } {
	return [ EpCreateUnitPositionTrigger $unit [lindex $tileRegion 0] [lindex $tileRegion 1] [lindex $tileRegion 2] [lindex $tileRegion 3] $flags ]
}

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
	}
	
	proc registerDialogs {} {
	}
	
	proc registerIncidents {} {
		variable bossDialogPos
		
		set incident			[ EpCreateBlockingActionIncident 0 0 1 ]
		
		EpAddTriggerToIncident	$incident [ createPosTrigTileRegion	[ EpGetHero ]	$bossDialogPos		0x001	]
		EpAddActionToIncident	$incident [ EpCreateDialogAction	"EpA213World::FinalBossQuest::bossDialog"		]
		
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
}
