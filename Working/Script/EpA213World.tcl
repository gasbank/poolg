source "Script/EpA213World_MirrorQuest.tcl"

namespace eval EpA213World {
	global pNPC_OldRat pEnemy_OldCat1 pEnemy_OldCat10 pEnemy_OldCat11
	variable pHeroUnit
	variable modelFilePath					"A213World.arn"
	variable gatePos						{ 14 66 }
	variable ratholPos						{ 12 50 }
	variable bossPos						{ 75 124 }
	variable bossBattleStartPos				{ 69 124 }
	variable startPos						$bossBattleStartPos
	variable dialogNameList					[ list	laserDialog openDialog NPC_OldRatDialog\
													NPC_OldCat1Dialog NPC_OldCat10Dialog ]
	
	proc init { curWorld } {
		variable world
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		
		set world				$curWorld;
	}

	proc enter {} {
		variable pHeroUnit
		variable gatePos
		variable ratholPos
		variable Box
		variable Mirror
		variable startPos
		variable bossPos
		
		global pNPC_OldRat pEnemy_OldCat1 pEnemy_OldCat10 pEnemy_OldCat11
		
		EpOutputDebugString " - [info level 0] called\n"
		
		set pHeroUnit				[ getHero $startPos ];
		set pEnemyUnit1				[ createEnemy 81 70 ];
		set pEnemyUnit2				[ createEnemy 82 60 ];
		set pEnemyUnit3				[ createEnemy 64 64 ];
		set pEnemyUnit4				[ createEnemy 52 72 ];
		set pEnemyUnit5				[ createEnemy 73 85 ];
		set pEnemyUnit6				[ createEnemy 77 93 ];
		set pEnemyUnit7				[ createEnemy 71 89 ];
		set pEnemyUnit8				[ createEnemy 64 97 ];
		set pEnemyUnit9				[ createEnemy 45 109 ];
		set pEnemyUnit10			[ createEnemy 81 103 ];
		set pFinalBoss				[ createEnemy2 $bossPos ];
		EpUnitSetArnMesh			$pFinalBoss "ChimpModel"
		
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetControllable	$pHeroUnit 1
		#EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		#EpCharacterSetCurHp			$pHeroUnit 10
		#EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpUnitSetColor			$pHeroUnit 255 0 255
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]

		EpUnitSetColor					$pEnemyUnit1 255 255 255
		EpCharacterSetStat				$pEnemyUnit1 4 1 3 1 1 1
		EpCharacterSetCurHp				$pEnemyUnit1 -1
		EpEnemySetRandomWalkable		$pEnemyUnit1 1
		EpCharacterSetBoundary			$pEnemyUnit1 78 67 84 73
		EpUnitSetArnMesh				$pEnemyUnit1 "DinoModel"

		EpUnitSetColor					$pEnemyUnit2 255 255 0
		EpCharacterSetStat				$pEnemyUnit2 4 1 3 1 1 1
		EpCharacterSetCurHp				$pEnemyUnit2 -1
		EpEnemySetRandomWalkable		$pEnemyUnit2 1
		EpCharacterSetBoundary			$pEnemyUnit2 79 57 85 63
		EpUnitSetArnMesh				$pEnemyUnit2 "DinoModel"

		EpUnitSetColor				$pEnemyUnit3 255 0 255
		EpCharacterSetStat			$pEnemyUnit3 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit3 -1
		EpEnemySetRandomWalkable		$pEnemyUnit3 1
		EpCharacterSetBoundary			$pEnemyUnit3 61 61 67 67
		EpUnitSetArnMesh				$pEnemyUnit3 "DinoModel"

		EpUnitSetColor				$pEnemyUnit4 0 255 255
		EpCharacterSetStat			$pEnemyUnit4 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit4 -1
		EpEnemySetRandomWalkable		$pEnemyUnit4 1
		EpCharacterSetBoundary			$pEnemyUnit4 49 69 55 75
		EpUnitSetArnMesh				$pEnemyUnit4 "DinoModel"

		EpUnitSetColor				$pEnemyUnit5 0 0 255
		EpCharacterSetStat			$pEnemyUnit5 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit5 -1
		EpEnemySetRandomWalkable		$pEnemyUnit5 1
		EpCharacterSetBoundary			$pEnemyUnit5 70 82 76 89
		EpUnitSetArnMesh				$pEnemyUnit5 "DinoModel"

		EpUnitSetColor				$pEnemyUnit6 0 255 0
		EpCharacterSetStat			$pEnemyUnit6 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit6 -1
		EpEnemySetRandomWalkable		$pEnemyUnit6 1
		EpCharacterSetBoundary			$pEnemyUnit6 74 90 80 96
		EpUnitSetArnMesh				$pEnemyUnit6 "DinoModel"

		EpUnitSetColor				$pEnemyUnit7 255 0 0
		EpCharacterSetStat			$pEnemyUnit7 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit7 -1
		EpEnemySetRandomWalkable		$pEnemyUnit7 1
		EpCharacterSetBoundary			$pEnemyUnit7 68 86 74 92
		EpUnitSetArnMesh				$pEnemyUnit7 "DinoModel"

		EpUnitSetColor				$pEnemyUnit8 128 128 128
		EpCharacterSetStat			$pEnemyUnit8 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit8 -1
		EpEnemySetRandomWalkable		$pEnemyUnit8 1
		EpCharacterSetBoundary			$pEnemyUnit8 61 94 67 100
		EpUnitSetArnMesh				$pEnemyUnit8 "DinoModel"

		EpUnitSetColor				$pEnemyUnit9 128 0 0
		EpCharacterSetStat			$pEnemyUnit9 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit9 -1
		EpEnemySetRandomWalkable		$pEnemyUnit9 1
		EpCharacterSetBoundary			$pEnemyUnit9 42 106 48 112
		EpUnitSetArnMesh				$pEnemyUnit9 "DinoModel"

		EpUnitSetColor				$pEnemyUnit10 0 0 0
		EpCharacterSetStat			$pEnemyUnit10 4 1 3 1 1 1
		EpCharacterSetCurHp			$pEnemyUnit10 -1
		EpEnemySetRandomWalkable		$pEnemyUnit10 1
		EpCharacterSetBoundary			$pEnemyUnit10 78 100 84 106
		EpUnitSetArnMesh				$pEnemyUnit10 "DinoModel"

		set pNPC_OldRat				[ createEnemy 4 59 ]
		EpUnitSetArnMesh			$pNPC_OldRat "PoolGModel"
		EpUnitSetColor				$pNPC_OldRat 128 128 128
		EpCharacterSetStat			$pNPC_OldRat 4 1 3 1 1 1
		EpCharacterSetCurHp			$pNPC_OldRat -1
		EpEnemySetTalkable			$pNPC_OldRat 1
		EpUnitSetName				$pNPC_OldRat "An Old Rat..."
		EpUnitSetNameVisible		$pNPC_OldRat 1

		set	pEnemy_OldCat1			[ createEnemy 21 65 ]
		EpUnitSetArnMesh			$pEnemy_OldCat1 "GwengYiModel"
		EpUnitSetColor				$pEnemy_OldCat1 128 128 128
		EpCharacterSetStat			$pEnemy_OldCat1 10 10 10 10 10 10
		EpCharacterSetCurHp			$pEnemy_OldCat1 100
		EpEnemySetTalkable			$pEnemy_OldCat1 1
		EpUnitSetName				$pEnemy_OldCat1 "No.1 Gatekeeper"
		EpUnitSetNameVisible		$pEnemy_OldCat1 1

		set	pEnemy_OldCat10			[ createEnemy 21 70 ]
		EpUnitSetArnMesh			$pEnemy_OldCat10 "GwengYiModel"
		EpUnitSetColor				$pEnemy_OldCat10 128 128 128
		EpCharacterSetStat			$pEnemy_OldCat10 10 10 10 10 10 10
		EpCharacterSetCurHp			$pEnemy_OldCat10 100
		EpEnemySetTalkable			$pEnemy_OldCat10 1
		EpUnitSetName				$pEnemy_OldCat10 "No.10 Gatekeeper"
		EpUnitSetNameVisible		$pEnemy_OldCat10 1

		set	pEnemy_OldCat11			[ createEnemy 21 60 ]
		EpUnitSetArnMesh			$pEnemy_OldCat11 "GwengYiModel"
		EpEnemySetTalkable			$pEnemy_OldCat11 1
		EpUnitSetName				$pEnemy_OldCat11 "No.11 Gatekeeper"
		EpUnitSetNameVisible		$pEnemy_OldCat11 1

		set Box					[createStructureObject 6 70];
		EpUnitSetArnMesh			$Box "PushableMirror"

		set Mirror				[createStructureObject 6 60];
		EpUnitSetArnMesh			$Mirror "PushableMirror"


		# Incidents ------------------------------------------------------------
		#Incident_BossGateOpen
		registerIncidentBox
		registerIncidentMirror
		registerIncidentOpen
		registerIncidentOpen2
		EpRegisterInitFadeInIncident
		registerIncident_TalkWithOldRat
		registerIncident_pEnemy_OldCat1
		
		registerMirrorQuest
	}
	
	
	proc Incident_GateOpen {} {
		variable pHeroUnit
		set animObjects [ list Blocking1 Blocking2 Blocking3 GateRight GateLeft GateCamera ]
		
		#set trigger			[ EpCreateUnitPositionTrigger	$pHeroUnit 23 75 21 57 0x001 ]
		set trigger			[ EpCreateNullTrigger ]
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
		set incident		[ EpCreateBlockingActionIncident $trigger 0 1 ]
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
	
	proc Incident_BossGateOpen {} {
		variable pHeroUnit
		set animObjects [ list Blocking1 Blocking2 Blocking3 GateRight GateLeft GateCamera ]
		
		set trigger			[ EpCreateUnitPositionTrigger	$pHeroUnit 42 57 42 71 0x001 ]
		set actions			[ list ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode LaserConeDeco  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Laser1  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Laser2  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode BossWall  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode BossWallEventCamera ] 1" ]
		lappend actions		[ EpCreateCameraAction			external BossWallEventCamera 0 ]
		lappend actions		[ EpCreateDelayAction			17000 ]
		lappend actions		[ EpCreateCameraAction			attach BossWallEventCamera 1500 ]
		set incident		[ EpCreateBlockingActionIncident $trigger 0 1 ]
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentBox {} {
		variable world
		variable Box
		variable Mirror
		
		set triggers		[ EpCreateUnitPositionTrigger $Box 9 70 -1 -1 0x001 ]
		set trigger	[ EpCreateUnitPositionTrigger $Mirror 9 60 -1 -1 0x100 ]
		lappend triggers	[ EpCreateReverseTrigger $trigger ]
		
		set actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode LaserConeDeco  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Laser1  ] 1" ]
		lappend actions		[ EpCreateDialogAction "EpA213World::laserDialog" ]

		set incident	[ EpCreateBlockingActionIncident 0 0 -1 ]

		foreach trig $triggers {
			EpAddTriggerToIncident $incident $trig
		}

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Box incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentMirror {} {
		variable world
		variable Box
		variable Mirror
		
		set trigger		[ EpCreateUnitPositionTrigger $Mirror 9 60 -1 -1 0x001 ]
		set actions		[ EpCreateFadeAction out 3500 ]
		lappend actions		[ EpCreateFadeAction in 3000 ]
		set incident	[ EpCreateBlockingActionIncident $trigger 0 -1 ]

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Mirror incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentOpen {} {
		variable world
		variable Box
		variable Mirror

		set triggers		[ EpCreateUnitPositionTrigger $Box 9 70 -1 -1 0x100 ]
		lappend triggers	[ EpCreateUnitPositionTrigger $Mirror 9 60 -1 -1 0x001 ]

		set actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Laser1  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Laser2  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode BossWall  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode BossWallEventCamera ] 1" ]
		lappend actions		[ EpCreateCameraAction			external BossWallEventCamera 0 ]
		lappend actions		[ EpCreateDelayAction			17000 ]
		lappend actions		[ EpCreateCameraAction			attach BossWallEventCamera 1500 ]
		lappend actions		[ EpCreateDialogAction "EpA213World::openDialog" ]
		
		set incident	[ EpCreateBlockingActionIncident 0 0 1 ]

		foreach trig $triggers {
			EpAddTriggerToIncident $incident $trig
		}

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Open incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc registerIncidentOpen2 {} {
		variable world
		variable Box
		variable Mirror

		set triggers		[ EpCreateUnitPositionTrigger $Box 9 70 -1 -1 0x001 ]
		lappend triggers		[ EpCreateUnitPositionTrigger $Mirror 9 60 -1 -1 0x100 ]
		
		set actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode LaserConeDeco  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Laser1  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode Laser2  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode BossWall  ] 1" ]
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode BossWallEventCamera ] 1" ]
		lappend actions		[ EpCreateCameraAction			external BossWallEventCamera 0 ]
		lappend actions		[ EpCreateDelayAction			17000 ]
		lappend actions		[ EpCreateCameraAction			attach BossWallEventCamera 1500 ]
		lappend actions		[ EpCreateDialogAction "EpA213World::openDialog" ]

		set incident	[ EpCreateBlockingActionIncident 0 0 1 ]

		foreach trig $triggers {
			EpAddTriggerToIncident $incident $trig
		}

		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Open2 incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}
		
	proc Incident_GateOpen_NewMethod {} {
			variable pHeroUnit
			set animObjectNames [ list Blocking1 Blocking2 Blocking3 GateRight GateLeft GateCamera ]
			set animObjects [ list ]
			foreach objName $animObjectNames {
				lappend xformables [ EpGetNode $objName ]
			}
			
			set trigger			[ EpCreateUnitPositionTrigger	$pHeroUnit 23 75 21 57 0x001 ]
			set actions			[ EpCreateControllableAction	$pHeroUnit 0 ]
			lappend actions		[ EpCreateCameraAction			external GateCamera 0 ]
			lappend actions		[ EpCreateStartAnimationAction	$animObjects ]
			lappend actions		[ EpCreateCameraAction			attach GateCamera 1500 ]
			lappend actions		[ EpCreateControllableAction	$pHeroUnit 1 ]
			
			set incident		[ EpCreateBlockingActionIncident $trigger 0 1 ]
			foreach act $actions {
				EpAddActionToIncident $incident $act
			}
	
			set incCount	[ EpRegisterIncident $incident ]
			EpOutputDebugString " - Incident count: $incCount\n"
		}

	# 설명 : 늙은 쥐와의 대화를 통해 A213에 얽힌 전설을 듣게 되고, 문지기를 해치우고 A213의 문을
	# 열기 위한 필수 아이템 상한 두부를 얻는다.
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

		# 문 열기 ---------------------------------------------------------------------------------
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
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

	namespace eval laserDialog {	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "SYSTEM"
	
		set dialog [ list\
			$player		"레이져가 나오고 있다."\
		];
	}

	namespace eval openDialog {	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "SYSTEM"
	
		set dialog [ list\
			$player		"레이져가 문을 부셔버렸다!"\
		];
	}


	namespace eval NPC_OldRatDialog {} {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "An Old Rat..."
		set	system "System"
	
		set dialog [ list\
			$npc		"자네도 Dark Lord Choi를 만나러 왔는가?"\
			$player		"....."\
			$npc		"자네의 깊고 어두운 코딩 스피릿을 감지하고 알 수 있었지.."\
			$npc		"왜 그에게 도전하려 하는겐가?"\
			$player		"복수하기 위해서 입니다."\
			$npc		"그렇군..."\
			$npc		"나도 자네처럼 젊은 때가 있었지."\
			$npc		"A213에는 전력과 냉방이 풍부하다는 소문이 있었어."\
			$npc		"동료들을 모아 모험을 떠났고, 여기에 도착했지. 자네처럼 말이야..."\
			$npc		"그리고 그를 만났지..."\
			$npc		"그는 정말 강했어! Ctr-C, Ctr-V 공격에 그는 눈 하나 깜박하지 않았어!"\
			$npc		"동료들은 모두 죽었고, 나는 비겁하게 혼자 살아 나왔다네."\
			$npc		"그리고 그 이후로 나는 계속 여기에서 살고 있지..."\
			$player		"왜 여기서 사십니까?"\
			$npc		"자네같은 친구를 위해서일세"\
			$npc		"내가 나온 그날 이후로, 저 문을 넘어가서 살아 돌아온 쥐는 없었다네."\
			$npc		"자네도 나처럼 모든 것을 잃지 말고, 돌아가게나."\
			$player		"저는 이미 대청소로 모든 것을 잃었습니다."\
			$player		"반드시 저 문을 지나서, 그에게 복수해야만 합니다."\
			$npc		"소용이 없구먼.."\
			$npc		"굳이 가겠다면, 내 부탁 하나만 들어주겠나?"\
			$player		"무엇입니까?"\
			$npc		"내가 쓰레기통에서 먹다 남은 두부 캔을 하나 주웠네"\
			$npc		"저기 북동쪽의 2번 문지기와 이것을 나누어 먹고 싶은데, 나는 늙고 병들어서 움직이기가 귀찮아."\
			$npc		"자네가 이것을 좀 가져다 주게."\
			$player		"네"\
			$system		"상한 두부를(을) 얻었습니다."\
		];	
	}

	namespace eval NPC_OldCat10Dialog {} {
	
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "Old Cat(Gatekeeper)"
		set	system "System"
	
		set dialog [ list\
			$npc		"두부 냄새가 나는군."\
			$player		"조금 드릴까요?"\
			$npc		"응 그러면 고맙겠어."\
			$player		"여기요."\
			$npc		"(냠냠...)"\
			$npc		"(쩝쩝...)"\
			$npc		"(우걱우걱...)"\
			$npc		"(꿀꺽!)"\
			$npc		"읔!!!!!!!!!!!"\
			$npc		"상했잖아!!!!"\
			$npc		"(털썩)"\
			$system		"당신은 쓰러진 문지기로부터 열쇠를 얻어서 구멍에 끼워 돌려 보았다."\
		];	
	}

	namespace eval NPC_OldCat1Dialog {} {
		
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "Old Cat(Gatekeeper)"
		set	system "System"
	
		set dialog [ list\
			$npc		"이게 무슨 냄새지?"\
			$player		"두부"\
			$npc		"괘씸하다! 내가 제일 싫어하는 두부를 가져오다니!"\
		];	
	}
}
