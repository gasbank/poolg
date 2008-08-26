namespace eval EpA213World {


	proc registerMirrorQuest {} {


		set Mirror				[createStructureObject 6 60];
		EpUnitSetArnMesh			$Mirror "PushableMirror"
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
		lappend actions		[ EpCreateScriptAction			"EpSetDoAnim [ EpGetNode BossWallEventCamera ] 1" ]
		lappend actions		[ EpCreateCameraAction			external BossWallEventCamera 0 ]
		lappend actions		[ EpCreateDelayAction			10000 ]
		lappend actions		[ EpCreateCameraAction			attach BossWallEventCamera 1500 ]
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
	
	#######################################################################################################
	
	
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
}
