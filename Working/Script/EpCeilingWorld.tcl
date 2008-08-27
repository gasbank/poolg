namespace eval EpCeilingWorld {
	set modelFilePath	"CeilingWorld.arn"
	set dialogNameList [ list introDialog GlooPDialog rGlooPDialog GlooPDialog2 GetGDialog SetGDialog weakCatDialog catDeadDialog ]
	global world hero npcGetg npcGloop pHeroUnit npcSetg weakCat
	variable startPos { 30 77 }
	variable mainQuest1_incident
	variable mainQuest2_incident

	proc registerIncidentInitTalk {} {
		global world hero npcGetg npcGloop pHeroUnit
		
		set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 30 77 30 77 0x100 ]
		set actions		[ EpCreateFadeAction "in" 3000 ]
		lappend actions [ EpCreateDialogAction "EpCeilingWorld::introDialog" ]
				
		set incident	[ EpCreateBlockingActionIncident $trigger 0 1 ]
		
		EpIncidentSetName $incident "Init Talk"
		EpIncidentSetControlDuringAction $incident 0
		
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "InitTalk incident"
		
		set incCount	[ EpRegisterIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc mainQuest {} {
		global world hero npcGetg npcGloop pHeroUnit npcSetg weakCat
		variable mainQuest1_incident
		variable mainQuest2_incident

		set incident	[ EpCreateSequentialIncident 1 ]

		EpAddTriggerToSequence	$incident	[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcGloop 0x001 ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::GlooPDialog" ]
		
		EpAddTriggerToSequence	$incident	[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $weakCat 0x001 ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::weakCatDialog" ]
		EpAddActionToSequence	$incident	[ EpCreateScriptAction "EpEnemySetTalkable $weakCat 0" ]

		EpAddTriggerToSequence	$incident	[ EpRemoveFlagTrigger $weakCat ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::catDeadDialog" ]

		EpAddTriggerToSequence	$incident	[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcGloop 0x001 ]
		EpAddActionToSequence	$incident	[ EpCreateDialogAction "EpCeilingWorld::GlooPDialog2" ]

		EpIncidentSetName	$incident "mainQuest incident"
		
		set incCount	[ EpRegisterSequentialIncident $incident ]
		EpOutputDebugString " - Incident count: $incCount\n"
	}

	proc Quest3 {} {
		global world hero npcGetg npcGloop pHeroUnit

		set trigger	 [ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcGloop 0x001 ]
		set actions	 [ EpCreateDialogAction "EpCeilingWorld::GlooPDialog" ]

		set incident	 [ EpCreateBlockingActionIncident $trigger 0 -1 ]
		EpIncidentSetName	$incident "Quest3 incident"
		foreach act $actions {
			EpAddActionToIncident $incident $act
		}

		EpIncidentSetName	$incident "Quest2 incident"

		## --------------------------------------------
		## Do not register a incident planned
		## to be invoked by EpCreateStartIncidentAndWaitAction!!!
		## --------------------------------------------
		#set incCount	[ EpRegisterIncident $incident ]
		#EpOutputDebugString " - Incident count: $incCount\n"

		EpOutputDebugString " - Quest2 Incident returned $incident\n"
		return $incident
	}

	proc Quest4 {} {
		global world hero npcGetg npcGloop pHeroUnit npcSetg

		set trigger			[ EpCreateUnitPositionWithTraceTrigger $pHeroUnit $npcSetg 0x001 ]
		set action			[ EpCreateDialogAction "EpCeilingWorld::SetGDialog" ]
		set incident		[ EpCreateBlockingActionIncident $trigger $action -1 ]
		EpIncidentSetName	$incident "Quest4 incident"

		## --------------------------------------------
		## Do not register a incident planned
		## to be invoked by EpCreateStartIncidentAndWaitAction!!!
		## --------------------------------------------
		##set incCount	[ EpRegisterIncident $incident ]
		##EpOutputDebugString " - Incident count: $incCount\n"
		
		EpOutputDebugString " - Quest3 Incident returned $incident\n"
		return $incident
	}

	proc init { curWorld } {
		global world hero npcGetg npcGloop npcSetg pHeroUnit weakCat
		variable startPos
		
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		set world					$curWorld;
		
		set pHeroUnit				[ getHero $startPos ];
		                                      # Health Will Coding Defence Sense Immunity
		EpCharacterSetStat			$pHeroUnit    7     5     7       5      5      5
		EpCharacterSetCurHp			$pHeroUnit 10
		EpCharacterSetCurCs			$pHeroUnit -1
		EpCharacterSetMoveDuration	$pHeroUnit [expr 0.2]
		EpUnitSetColor				$pHeroUnit 255 0 255
		#EpUnitSetRotX				$pHeroUnit [ToRadian -90]
		#EpUnitSetRotZ				$pHeroUnit [ToRadian 90]
		EpUnitSetPosZ				$pHeroUnit -[EpUnitGetUpperRightZ $pHeroUnit]
		EpUnitSetArnMesh			$pHeroUnit "PoolGModel"
        
		set npcGetg					[ createEnemy 34 79 ];
		EpUnitSetColor				$npcGetg 128 128 0
		EpCharacterSetStat			$npcGetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcGetg -1
		EpEnemySetTalkable			$npcGetg 1
		EpUnitSetArnMesh			$npcGetg "PoolGModel"
		EpUnitSetName				$npcGetg "GetG"
		EpUnitSetNameVisible		$npcGetg 1
		
		set npcGloop				[ createEnemy 30 82 ];
		EpUnitSetColor				$npcGloop 0 255 255
		EpCharacterSetStat			$npcGloop 3 1 4 1 1 1
		EpCharacterSetCurHp			$npcGloop -1
		EpEnemySetTalkable			$npcGloop 1
		EpUnitSetArnMesh			$npcGloop "PoolGModel"
		EpUnitSetName				$npcGloop "Gloop"
		EpUnitSetNameVisible		$npcGloop 1

		set npcSetg					[ createEnemy 30 72 ];
		EpUnitSetColor				$npcSetg 128 128 0
		EpCharacterSetStat			$npcSetg 4 1 3 1 1 1
		EpCharacterSetCurHp			$npcSetg -1
		EpEnemySetTalkable			$npcSetg 1
		EpUnitSetArnMesh			$npcSetg "PoolGModel"
		EpUnitSetName				$npcSetg "SetG"
		EpUnitSetNameVisible		$npcSetg 1

		set weakCat					[ createEnemy 42 75 ];
		EpUnitSetColor				$weakCat 0 0 0 
		EpCharacterSetStat			$weakCat 2 2 2 2 2 2
		EpCharacterSetCurHp			$weakCat -1
		EpEnemySetTalkable			$weakCat 1
		EpUnitSetArnMesh			$weakCat "GwengYiModel"

		registerIncidentInitTalk
		mainQuest
			
		createWarpPosition			"EpRoomWorld" STOP 25 82
	}

	proc enter {} {
		EpOutputDebugString " - [info level 0] called\n"
	}
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}




	namespace eval introDialog {
		
		set region [ list 0 0 -1 -1 ]; ;# left, top, right, bottom
		set oneTime 1;
		
		set player "PoolG"
		set npc1 "GlooP"
		set npc2 "GetG"
		
		set dialog [ list\
			$npc1		"PoolC에서 대청소를 해버리는 바람에, 우리들의 삶의 터전이 좁아저 버렸다네"\
			$player		"뭬야!"\
			$npc1		"게다가 앞으론 우리들을 살 수 없도록 언제나 깨끗하게 유지하겠다고 하더군"\
			$player		"맙소사! 난 가만히 있을 수 없어"\
			$npc2		"제발 그들을 말려주게"\
			$player		"이대로 가만히 있을 순 없지. 다녀오겠네"\
		];
	}

	namespace eval GlooPDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GlooP"
	
		set dialog [ list\
			$npc		"동아리 방에는 '괭이' 라고 불리는 생물이 돌아다니는 듯 하네"\
			$player		"뭐하는 녀석인가?"\
			$npc		"KKK라고 불리는 사람이 그린 고양이라고 하던데...."\
			$player		"그림이 돌아다닐리가 없잖아"\
			$npc		"저~기에 있으니 한번 물어보시게"\
		];	
	}

	namespace eval rGlooPDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GlooP"
	
		set dialog [ list\
			$npc		"물어보고 오라니깐?"\
			$player		"가서 싸우는게 아니라?"\
			$npc		"들켰나!"\
		];	
	}

	namespace eval GlooPDialog2 {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GlooP"
	
		set dialog [ list\
			$npc		"잘했네"\
			$player		"아 예"\
		];	
	}

	namespace eval GetGDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "GetG"
	
		set dialog [ list\
			$npc		"변수는 Private에 넣은뒤, Get 함수로 불러온다면 누가 불러가는지 추적이 편해진다네"\
			$player		"그것은 코딩의 기초가 아닌가"\
			$npc		"하지만 이건 C에선 지원하지 않지"\
			$player		"도대체 왜 아직도 C를 쓰는지 모르겠어"\
			$npc		"GlooP에게 물어보시게"\
		];	
	}

	namespace eval SetGDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set npc "SetG"
	
		set dialog [ list\
			$npc		"안녕"\
			$player		"어쩌라구요♡"\
		];	
	}

	namespace eval weakCatDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
		set enemy "GwengYi"
	
		set dialog [ list\
			$enemy		"여긴 어디지? 난 누구?"\
			$player		"..... 뭐야 이건"\
			$enemy		"우오!!!!!!!!!!!!!!!"\
			$player		"뭐, 뭐냐!"\
		];	
	}

	namespace eval catDeadDialog {
	
		set region [ list 0 0 0 0 ]; ;# left, top, right, bottom
		set oneTime 0;
	
		set player "PoolG"
	
		set dialog [ list\
			$player		"적장, 물리쳤다!"\
		];	
	}
}
