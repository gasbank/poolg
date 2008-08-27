# Example code

proc registerIncidentUnClear {} {
	variable world
	variable pHeroUnit
	variable pTestEnemyUnit
	
	set trigger		[ EpCreateUnTotalAnnihilationTrigger $world ]
	set action		[ EpCreateDialogAction "EpDialogReturn" ]
	set incident		[ EpCreateNonblockingActionIncident $trigger $action -1 ]

	set trigger2		[ EpCreateUnitPositionTrigger $pHeroUnit 50 33 59 33 0x001 ]
	set action2		[ EpCreateUnitMoveAction $pHeroUnit "UP" ]
	set action3		[ EpCreateUnitMoveAction $pTestEnemyUnit "DOWN" ]

	EpAddTriggerToIncident $incident $trigger2
	EpAddActionToIncident $incident $action2
	EpAddActionToIncident $incident $action3

	EpIncidentSetName	$incident "Unclear incident"

	set incCount	[ EpRegisterIncident $incident ]
	EpOutputDebugString " - Incident count: $incCount\n"
}

proc registerAlertIncident {} {
	variable pHeroUnit

	set trigger		[ EpCreateUnitPositionTrigger $pHeroUnit 26 73 26 73 0x001 ]
	set action		[ EpCreateFlickerAction 10000 500 255 0 0 ]
	set incident	[ EpCreateNonblockingActionIncident $trigger $action 1 ]

	EpIncidentSetName	$incident "alert incident"

	set incCount	[ EpRegisterIncident			$incident ]
	EpOutputDebugString " - Incident count: $incCount\n"
}