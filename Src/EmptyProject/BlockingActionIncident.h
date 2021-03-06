﻿#pragma once

#include "SingleIncident.h"

/**
@brief 정의된 Action을 순차적으로 실행하는 Incident

*/
class BlockingActionIncident : public SingleIncident
{
public:
	BlockingActionIncident( int trigCount );
	BlockingActionIncident( Trigger* trigger, Action* action, int trigCount );

	virtual bool update( double dTime, float fElapsedTime );
	virtual bool isFinished() const { return ( getLeastOnetime() && !isActivated() && m_curActionIt == m_action.end() ); }
protected:
	virtual void activate();
private:
	ActionList::iterator m_curActionIt;
};

SCRIPT_FACTORY( BlockingActionIncident )