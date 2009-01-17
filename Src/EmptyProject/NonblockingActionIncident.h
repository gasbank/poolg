#pragma once
#include "SingleIncident.h"

/**
@brief 정의된 Action을 동시에 실행하는 Incident

*/
class NonblockingActionIncident : public SingleIncident
{
public:
	NonblockingActionIncident( int trigCount );
	NonblockingActionIncident( Trigger* trigger, Action* action, int trigCount );

	virtual bool update( double dTime, float fElapsedTime );
	virtual bool isFinished() const { return getLeastOnetime(); }

};

SCRIPT_FACTORY( NonblockingActionIncident )