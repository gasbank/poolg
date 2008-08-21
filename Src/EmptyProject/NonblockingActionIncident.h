#pragma once
#include "Incident.h"

class NonblockingActionIncident : public Incident
{
public:
	NonblockingActionIncident( int trigCount );
	NonblockingActionIncident( Trigger* trigger, Action* action, int trigCount );

	virtual bool update( double dTime, float fElapsedTime );
	virtual bool isFinished() const { return m_LeastOnetime; }

};

SCRIPT_FACTORY( NonblockingActionIncident )