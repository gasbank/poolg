#
# EpA213World_FinalBossQuest.tcl
#
# 목표:
# 보스를 죽여라!
#
# NPC:
# 보스
#
# 오브젝트:
# 없음
#
# 개요:
# 보스는 풀쥐를 기다리고 있었다. 이제 세계의 평화를 위해 우리의 풀쥐는
# 보스를 죽여야만 한다!!!
#
# 성과물:
# 세계의 평화
#


namespace eval EpA213World::FinalBossQuest {
	variable bossPos			{ 75 124 }					# 최종 보스 위치
	variable bossDialogPos		{ 70 118 70 130 }			# 최종 보스와의 대화 위치
	variable exitPos			{ 8 51 17 51 }				# 출구 위치
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
			$npc		"후후후후... 드디어 도착했군."\
			$player		"네가 우리 설치류의 평화를 방해한 놈이구나!"\
			$npc		"설치류의 평화? 설치류의 평화를 방해한 적은 없다."\
			$player		"무슨 헛소리야. 니놈이 우리의 안락한 환경을 살기 힘들게 바꿔놓고 이제는\
						그것도 모자라 우리의 보금자리까지 넘보고 있잖아!"\
			$npc		"그건 설치류의 평화와는 무관한 이야기지. 나는 그저 설치류따위가 이 세계에서\
						사라졌으면 하는 작은 소망을 가진 평범한 인간일 뿐이다."\
			$player		"한번은 우리의 아이를 잡아다가 동물원의 쥐마냥 여러 사람의 구경거리가 되게 만든 적도\
						있었지. 그것은 우리와의 전쟁을 선포한 것과 마찬가지다."\
			$npc		"그렇게 받아들였다니 최소한의 이해력은 있구나. 그리고 그 전쟁을 이제 끝낼 때가 왔구나."\
			$player		"전쟁을 해서 뭘 얻겠다는 것이지? 설치류를 말살시킬 수는 없는거야!"\
			$npc		"나라면 가능하다."\
			$player		"결국엔 서로 상처만 입을 뿐이야. 인간이나 설치류나. 최대한 평화적으로 해결하길 원한다."\
			$npc		"이제와서 평화라니 뇌가 녹고 있기라도 한거냐. 우리가 마리당 2만원을 주고 분양받은\
						수많은 괭이들의 영혼은 어떻게 보상할 것이냐. 무고한 괭이가 많이도 죽어나갔다. 네놈의\
						더러운 손에 말이지!"\
			$player		"애초에 괭이 따위를 사들인게 잘못 아니냐. 그리고 더욱 더 애초에 지금의 동아리방 자리는\
						우리 설치류의 보금자리였다. 먼저 침략을 시작한건 너희들이야!"\
			$npc		"침략이 아니라 발전이겠지. 우리는 더럽고 추악한 설치류로 득실거리는 그 허망한 공터를\
						인간들이 편안하게 생활할 수 있는 곳으로 일구어냈다."\
			$player		"그게 침략이 아니고 뭐란 말이지? 우리는 지금까지 너무나도 많이 참아왔다. 그런데도\
						불구하고 너희 인간들은 우리의 인내심을 하루가 머다하고 시험했어."\
			$player		"최대한 평화적으로 해결하려고 했건만 이제 그 끝을 봐야겠구나!"\
			$npc		"아까부터 평화 평화 해대는대 진정한 평화가 뭔지는 너희 설치류가 이 지구상에서 말끔히\
						사라지는 날에 알게 될 것이다."\
			$player		"닥쳐라!"
		];
	}
	
	namespace eval bossDeadDialog {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 1;

		set player "PoolG"
		set npc "Boss"
		set system "System"

		set dialog [ list\
			$player		"이야압~~~~~~~"\
			$npc		"으읔.으......."\
			$player		"죽어라!!!!"\
			$npc		"크으으으으읔..."\
			$npc		"이 원...수는 반..드..시 갚아주마.."\
			$player		"내가 그렇게도 평화적으로 해결하려고 했거늘... 아쉽구나.."\
			$npc		"다... 찌..ㅈ어 버리..겠.... 읔"\
			$player		"마지막 가는 길이라도 편안하게 가거라. 인간이여."\
			$npc		"나..는... 혼.자 죽을 수... 없다.. 크으으흐흐흐"\
			$player		"인간이나 쥐나 어차피 한번 죽는 생이거늘 너무 슬퍼하지는 말거라."\
			$npc		"이..렇게 된 이상.. 너도 이 컴실과 함께 죽을 수 밖에 없다..."\
			$player		"뭐라고!"\
			$npc		"후..후... 내세에서 다시 보자꾸나.."\
			$system		"폭파 5분 전입니다."\
			$player		"...이런 쉩... 갑자기 왜 할리우드 스타일로 바뀌고 난리야!"\
			$player		"어쨌든 여길 빨리 빠져나가야겠다."\
			$npc		"크으.. 넌 죽어야만 해... 으으읔.."\
		];
	}
	
	namespace eval exitA213Dialog {
		set region [ list 0 0 0 0 ] ;# left, top, right, bottom
		set oneTime 1;

		set player "PoolG"

		set dialog [ list\
			$player		"이제 모든 것이 끝이구나..."\
		];
	}
}
