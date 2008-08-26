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

proc createPosTrigTileRegion { unit tileRegion { flags 0x001 } } {
	return [ EpCreateUnitPositionTrigger $unit [lindex $tileRegion 0] [lindex $tileRegion 1] [lindex $tileRegion 2] [lindex $tileRegion 3] $flags ]
}

namespace eval EpA213World::FinalBossQuest {
	variable bossPos			{ 75 124 }			# 최종 보스 위치
	variable bossDialogPos		{ 70 118 70 130 }			# 최종 보스와의 대화 위치
	
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
}
