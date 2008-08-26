source "Script/EpA213World_MirrorQuest.tcl"
source "Script/EpA213World_LegendOfDinosaurQuest.tcl"
source "Script/EpA213World_OldRatandGatekeeperQuest.tcl"

namespace eval EpA213World {
	variable pHeroUnit
	variable modelFilePath					"A213World.arn"
	variable gatePos						{ 14 66 }
	variable ratholPos						{ 12 50 }
	variable bossPos						{ 75 124 }
	variable bossBattleStartPos				{ 69 124 }
	variable startPos						$ratholPos
	variable dialogNameList					[ list	laserDialog openDialog oldRatDialog\
													oldCat1Dialog oldCat10Dialog\
													goodDinosaurDialog0 goodDinosaurDialog1\
													evilRatDialog bigRigsAssaultDialog]

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
		
		
		set pFinalBoss		 			[ createEnemy2 $bossPos ];
		EpUnitSetArnMesh				$pFinalBoss "ChimpModel"
		



		# Units	----------------------------------------------------------------
		registerUnitsOldRatandGatekeeper
		registerUnitsLegendOfDinosaur

		# Incidents ------------------------------------------------------------
		#registerIncidentBox
		#registerIncidentMirror
		#registerIncidentOpen
		#registerIncidentOpen2
		
		registerIncident_TalkWithOldRat
		registerIncident_pOldCat1
		registerIncident_LegendOfDinosaur
		
		registerMirrorQuest
		
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
