source "Script/EpA213World_MirrorQuest.tcl"
source "Script/EpA213World_FinalBossQuest.tcl"
source "Script/EpA213World_LegendOfDinosaurQuest.tcl"
source "Script/EpA213World_OldRatandGatekeeperQuest.tcl"

namespace eval EpA213World {
	variable pHeroUnit
	variable modelFilePath					"A213World.arn"
	variable gatePos						{ 14 66 }
	
	# Start Positions
	variable ratholPos						{ 12 50 }
	variable bossBattleStartPos				{ 53 126 }
	variable mirrorQuestStartPos			{ 51 80 }
	
	variable startPos						$ratholPos
	
	variable dialogNameList					[ list ]

	proc init { curWorld } {
		EpOutputDebugString " - [info level 0] called / curWorld: $curWorld\n"
		
		
	}
	

	
	proc enter {} {
		variable gatePos
		variable ratholPos
		variable Box
		variable Mirror
		variable startPos
		variable bossPos
		variable pHeroUnit					[ getHero $startPos ]
		
		EpOutputDebugString " - [info level 0] called\n"
		#EpHeroSetEncounterEnemy				0
		
		createEnemyDino { 81 70 }
		createEnemyDino { 82 60 }
		createEnemyDino { 64 64 }
		createEnemyDino { 52 72 }
		createEnemyDino { 73 85 }
		createEnemyDino { 77 93 }
		createEnemyDino { 71 89 }
		createEnemyDino { 64 97 }
		createEnemyDino { 45 109 }
		createEnemyDino { 81 103 }
		
		# Units	----------------------------------------------------------------

		# Incidents ------------------------------------------------------------
		#registerIncidentBox
		#registerIncidentMirror
		#registerIncidentOpen
		#registerIncidentOpen2
		
		OldRatandGatekeeperQuest::register
		LegendOfDinosaurQuest::register
		MirrorQuest::register
		FinalBossQuest::register
		
		EpRegisterInitFadeInIncident
	}
	
		

	
	
	proc leave {} {
		EpOutputDebugString " - [info level 0] called\n"
	}

	########################################################################################################
	
	proc createEnemyDino { tilePos } {
		set dino [ createEnemy2 $tilePos ]
		
		EpUnitSetColor					$dino 255 0 255
		EpCharacterSetStat				$dino 4 1 3 1 1 1
		EpCharacterSetCurHp				$dino -1
		EpEnemySetRandomWalkable		$dino 1
		EpUnitSetArnMesh				$dino "DinoModel"
		
		return $dino
	}
	
}
