namespace eval EpA213World {
# -----------------------------------------------------------------------------------------
	# 제목 : 늙은 쥐의 전설과 문지기
	# 설명 : 늙은 쥐와의 대화를 통해 A213에 얽힌 전설을 듣게 되고, 문지기를 해치우고 A213의 문을
	# 열기 위한 필수 아이템 상한 두부를 얻는다.
	# registerIncident_TalkWithOldRat
	# registerIncident_pEnemy_OldCat1 
	proc registerUnitsOldRatandGatekeeper {} {
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
	}

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