#pragma once

#include "Incident.h"

class BlockingActionIncident : public Incident
{
public:
	BlockingActionIncident( int trigCount );
	BlockingActionIncident( Trigger* trigger, Action* action, int trigCount );

	virtual bool update( double dTime, float fElapsedTime );
	virtual bool isFinished() const { return ( m_LeastOnetime && !isActivated() && m_curActionIt == m_action.end() ); }
protected:
	virtual void activate();
private:
	ActionList::iterator m_curActionIt;
};

SCRIPT_FACTORY( BlockingActionIncident )