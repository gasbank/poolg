#
# EpA213World_MirrorQuest.tcl
#
# ��ǥ:
# ���� �湮�� �����.
#
# NPC:
# ����
#
# ������Ʈ:
# �� �� �ִ� �ſ� 1��, �� �� �ִ� ���� 1��
#
# ����:
# ������ �ִ� ���� ���� ���� ���ؼ��� �������� ������ ������ �ʿ��ϴ�.
# �������� �������� �Ѵ� ����ġ �����ٰ� ���ڸ� �÷� ������ �ȴ�.
# �������� �߻�Ǵ� �͸����δ� ���� �湮�� ������ �ʴ´�.
# �ſ��� �� ��ġ�� �Ű� �ξ�� �������� �ſ￡ ���� �ݻ�Ǿ� ������
# ���� �湮���� �����ϰ� �ȴ�.
#
# ������:
# ����
#

proc createPosTrigOneTile { unit tilePos { flags 0x001 } } {
	return [ EpCreateUnitPositionTrigger $unit [lindex $tilePos 0] [lindex $tilePos 1] -1 -1 $flags ]
}

namespace eval EpA213World::MirrorQuest {

	variable mirrorPos			{ 51 85 }			# �ſ� �ʱ� ��ġ
	variable mirrorTargetPos	{ 51 88 }			# �ſ��� ���������� �־��� ��ġ
	variable laserBoxPos		{ 81 91 }			# �������� �� �� �ִ� ����
	variable laserBoxTargetPos	{ 81 88 }			# �������� �� �� �ִ� ���ڸ� �־��� ��ġ

	variable mirror
	variable laserBox

	proc register {} {
		registerObjects
		registerDialogs
		registerIncidents
		
		EpOutputDebugString " - MirrorQuest registered"
	}
	
	proc registerObjects {} {
		variable mirrorPos
		variable laserBoxPos
		variable mirror			[ createStructureObject2		$mirrorPos		]
		variable laserBox		[ createStructureObject2		$laserBoxPos	]
				
		EpUnitSetArnMesh		$mirror			"PushableMirror"
		EpUnitSetArnMesh		$laserBox		"PushableBox"
	}
	
	proc registerDialogs {} {	
	}
	
	proc registerIncidents {} {
		variable mirrorTargetPos
		variable laserBoxTargetPos
		variable mirror
		variable laserBox
		
		set incident			[ EpCreateBlockingActionIncident 0 0 1 ]
		
		EpAddTriggerToIncident	$incident [ createPosTrigOneTile	$mirror			$mirrorTargetPos	0x101	]
		EpAddTriggerToIncident	$incident [ createPosTrigOneTile	$laserBox		$laserBoxTargetPos	0x101	]
		EpAddActionToIncident	$incident [ EpCreateScriptAction	"EpA213World::MirrorQuest::doAnim"			]
		EpAddActionToIncident	$incident [ EpCreateCameraAction	external BossWallEventCamera 0				]
		EpAddActionToIncident	$incident [ EpCreateDelayAction		17000										]
		EpAddActionToIncident	$incident [ EpCreateCameraAction	attach xxx 0								]
		
		EpRegisterIncident $incident
	}
	
	proc doAnim {} {
		EpSetDoAnim [ EpGetNode Laser1					]	1
		EpSetDoAnim [ EpGetNode Laser2					]	1
		EpSetDoAnim [ EpGetNode BossWall				]	1
		EpSetDoAnim [ EpGetNode BossWallEventCamera		]	1
	}
	
}
