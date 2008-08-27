#
# EpA213World_LegendOfDinosaurQuest.tcl
#
# 제목 : 공룡의 전설
#
# 설명 : A213의 공룡들은 사실 원래 쥐였다. 이 쥐들은 A213에 들어왔다가 최재영을 만나서
# 본디의 코딩 스피릿의 어두운 면을 모두 잃고서 A213을 배회하는 괴물들이 되고 말았다.
# 하지만 아직 코딩 스피릿의 어두운 면을 다 잃지 않은 한 공룡(43, 54)이 주인공에게 "Big Rigs Limited
# Special Package"을 찾아오면 자신이 다시 쥐로 돌아수 있다고 주인공에게 부탁을 한다.
# 그리고 이 아이템은 A213 남서쪽 공간의 한 쥐(25, 45)가 가지고 있다.
# 주인공은 "Big Rigs..."를 가져오면서 다른 쥐들의 습격(12, 56)도 받는다.
#

namespace eval EpA213World::LegendOfDinosaurQuest {
	global pOldRat pOldCat1 pOldCat10 pOldCat11\
			pGoodDinosaur pEvilRat

	proc register {} {
		registerUnitsLegendOfDinosaur		
		registerIncident_LegendOfDinosaur
	}

	proc registerUnitsLegendOfDinosaur {} {
		global pOldRat pOldCat1 pOldCat10 pOldCat11\
		pGoodDinosaur pEvilRat

		set pGoodDinosaur		[ createEnemy 44 74 ]
		EpUnitSetColor				$pGoodDinosaur 255 128 0
		EpUnitSetArnMesh			$pGoodDinosaur "DinoModel"
		EpEnemySetTalkable			$pGoodDinosaur 1
		EpUnitSetName				$pGoodDinosaur "Gentle Dinosaur"
		EpUnitSetNameVisible		$pGoodDinosaur 1

		set pEvilRat			[ createEnemy 25 45 ]
		EpUnitSetColor				$pEvilRat	255 0 0
		EpUnitSetArnMesh			$pEvilRat "PoolGModel"
		EpEnemySetTalkable			$pEvilRat 1
		EpUnitSetName				$pEvilRat "Evil Rat"
		EpUnitSetNameVisible		$pEvilRat	1
	}

	proc registerIncident_LegendOfDinosaur {} {
		variable pHeroUnit
		global	pGoodDinosaur pEvilRat

		set seqIncident		[ EpCreateSequentialIncident 1 ]

		set triggerA0		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pGoodDinosaur 0x001 ]
		set actionA0		[ EpCreateDialogAction "EpA213World::LegendOfDinosaurQuest::goodDinosaurDialog0" ]

		set triggerB0		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pEvilRat 0x001 ]
		set actionB0		[ EpCreateDialogAction "EpA213World::LegendOfDinosaurQuest::evilRatDialog" ]
		set actionB1		[ EpCreateScriptAction "EpEnemySetTalkable $pEvilRat 0" ]

		set triggerC0		[ EpCreateUnitPositionTrigger $EpA213World::pHeroUnit 10 54 14 53 0x001 ]
		set actionC1		[ EpCreateDialogAction "EpA213World::LegendOfDinosaurQuest::bigRigsAssaultDialog" ]
		set actionC2		[ EpCreateScriptAction "createEnemy 11 55" ]
		set actionC3		[ EpCreateScriptAction "createEnemy 13 55" ]

		set triggerD0		[ EpCreateUnitPositionWithTraceTrigger $EpA213World::pHeroUnit $pGoodDinosaur 0x001 ]
		set actionD0		[ EpCreateDialogAction "EpA213World::LegendOfDinosaurQuest::goodDinosaurDialog1" ]
		set actionD1		[ EpCreateScriptAction "EpUnitSetRemoveFlag $pGoodDinosaur 1" ]
		# TODO: 착한 공룡이 사라지거나 쥐로 변하거나 해야 한다.

		EpAddTriggerToSequence	$seqIncident $triggerA0
		EpAddActionToSequence	$seqIncident $actionA0

		EpAddTriggerToSequence	$seqIncident $triggerB0
		EpAddActionToSequence	$seqIncident $actionB0
		EpAddActionToSequence	$seqIncident $actionB1

		EpAddTriggerToSequence	$seqIncident $triggerC0
		EpAddActionToSequence	$seqIncident $actionC1
		EpAddActionToSequence	$seqIncident $actionC2
		EpAddActionToSequence	$seqIncident $actionC3

		EpAddTriggerToSequence	$seqIncident $triggerD0
		EpAddActionToSequence	$seqIncident $actionD0
		EpAddActionToSequence	$seqIncident $actionD1

		EpIncidentSetName	$seqIncident "LegendOfDinsaur sequential incident"

		set incCount		[ EpRegisterSequentialIncident $seqIncident ]
		
		EpOutputDebugString " - LegendOfDinsaur Incident returned $seqIncident\n"
		return $seqIncident
	}

	namespace eval goodDinosaurDialog0 {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "Gentle Dinosaur"
		set system "System"

		set dialog [ list\
			$npc		"이봐요"\
			$player		"공룡이 말을 하다니!"\
			$npc		"도와주세요."\
			$player		"무엇을 도와드릴까요?"\
			$npc		"여기 모든 공룡들은 원래 코딩 스피릿의 어두운 면이 충만한 당신과 같은 쥐들이었죠"\
			$npc		"하지만 A213에 들어와 Dark Lord Choi의 무자비한 디자인 패턴 공격으로 코딩 스피릿을 모두 잃고"\
			$npc		"이렇게 괴물이 되어 이곳을 배회하게 된 것이에요..."\
			$player		"그렇군."\
			$npc		"저도 곧 이성을 잃고 배회하는 괴물이 될 거에요."\
			$npc		"하지만 그게 있다면..."\
			$player		"그거라면, Big Rigs Limited Special Package를 말하는 건가?"\
			$npc		"바로 그거에요!"\
			$npc		"A213을 나가서 남쪽으로 가면 넓은 공터가 있어요."\
			$npc		"거기에 제가 Big Rigs Limited Special Package를 숨겨 놓았어요."\
			$player		"알겠네."\
		];
	}

	namespace eval evilRatDialog {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "Evil Rat"
		set system "System"

		set dialog [ list\
			$player		"그 Big Rigs Limited Special Package는 어디서 난건가?"\
			$npc		"My pressure..."\
			$player		"내 놓으시지"\
			$npc		"안돼! 이건 내거야!!"\
		];
	}

	namespace eval bigRigsAssaultDialog {} {
	set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "Assassin Rat"
		set system "System"

		set dialog [ list\
			$npc		"Big Rigs Limited Special Package를 가지고 있군."\
			$player		"누구냐!"\
			$npc		"죽어줘야겠다."\
		];
	}

	namespace eval goodDinosaurDialog1 {} {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 0;

		set player "PoolG"
		set npc "Gentle Dinosaur"
		set system "System"

		set dialog [ list\
			$npc		"가져오셨군요!"\
			$npc		"정말 고마워요. 저는 이제 다시 쥐로 돌아갈 수 있게 되었어요."\
			$player		"별말씀을"\
		];
	}
}
