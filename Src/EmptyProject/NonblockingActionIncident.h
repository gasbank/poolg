#pragma once
#include "SingleIncident.h"

class NonblockingActionIncident : public SingleIncident
{
public:
	NonblockingActionIncident( int trigCount );
	NonblockingActionIncident( Trigger* trigger, Action* action, int trigCount );

	virtual bool update( double dTime, float fElapsedTime );
	virtual bool isFinished() const { return getLeastOnetime(); }

};

SCRIPT_FACTORY( NonblockingActionIncident )