#
# EpA213World_MirrorQuest.tcl
#
# 목표:
# 보스 방문을 열어라.
#
# NPC:
# 없음
#
# 오브젝트:
# 밀 수 있는 거울 1개, 밀 수 있는 상자 1개
#
# 개요:
# 보스가 있는 방의 문을 열기 위해서는 레이저와 상자의 조작이 필요하다.
# 레이저는 레이저를 켜는 스위치 위에다가 상자를 올려 놓으면 된다.
# 레이저가 발사되는 것만으로는 보스 방문이 열리지 않는다.
# 거울을 제 위치로 옮겨 두어야 레이저가 거울에 의해 반사되어 적절히
# 보스 방문까지 도달하게 된다.
#
# 성과물:
# 없음
#

proc createPosTrigOneTile { unit tilePos { flags 0x001 } } {
	return [ EpCreateUnitPositionTrigger $unit [lindex $tilePos 0] [lindex $tilePos 1] -1 -1 $flags ]
}

namespace eval EpA213World::MirrorQuest {

	variable mirrorPos			{ 51 85 }			# 거울 초기 위치
	variable mirrorTargetPos	{ 51 88 }			# 거울을 최종적으로 둬야할 위치
	variable laserBoxPos		{ 81 91 }			# 레이저를 켤 수 있는 상자
	variable laserBoxTargetPos	{ 80 88 }			# 레이저를 결 수 있는 상자를 둬야할 위치

	variable mirror
	variable laserBox

	proc register {} {
		registerObjects
		registerDialogs
		registerIncidents
		
		setAnimEnvironment
		
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
		EpAddActionToIncident	$incident [ EpCreateDelayAction		16000										]
		EpAddActionToIncident	$incident [ EpCreateCameraAction	attach xxx 0								]
		
		EpRegisterIncident $incident
	}
	
	proc setAnimEnvironment {} {
		EpMeshSetVisible [ EpGetNode Laser1			] 0
		EpMeshSetVisible [ EpGetNode Laser2			] 0
		EpMeshSetVisible [ EpGetNode LaserConeDeco	] 0
	}
	proc doAnim {} {
		EpMeshSetVisible [ EpGetNode Laser1			] 1
		EpMeshSetVisible [ EpGetNode Laser2			] 1
		EpMeshSetVisible [ EpGetNode LaserConeDeco	] 1
		
		EpSetDoAnim [ EpGetNode Laser1					]	1
		EpSetDoAnim [ EpGetNode Laser2					]	1
		EpSetDoAnim [ EpGetNode BossWall				]	1
		EpSetDoAnim [ EpGetNode LaserConeDeco			]	1
		EpSetDoAnim [ EpGetNode BossWallEventCamera		]	1
	}
	
}
