#pragma once
#include "Incident.h"

class StartIncidentAction;


/**
@brief 다수 Trigger -- 다수 Action을 묶어주는 단위 Incident

다수의 Trigger가 AND 관계로 모두 만족되었을 때 다수의 Action이 실행되게 해 주는
Incident입니다.

다수의 Action 이 실행되는 방법은 두 가지가 있습니다.
Action 이 동시에 실행되게 하는 방법이 있고 하나 하나씩
차례로 실행되게 하는 방법이 있습니다. 전자의 경우
NonblockingActionIncident 를 사용하고 후자의 경우
BlockingActionIncident 를 사용합니다.
*/
class SingleIncident :
	public Incident
{
public:
							SingleIncident( int trigCount );
							SingleIncident( Trigger* trigger, Action* action, int trigCount );
	virtual					~SingleIncident(void);

	// Incident::update() returns false when there are some remaining
	// updates of action or one of triggers are not satisfied.
	// Returns true when all actions are finished.
	virtual bool			update( double dTime, float fElapsedTime );
	virtual void			printDebugInfo() const;
	virtual void			printDebugInfoDetailed() const;
	virtual void			release();

	void					addTrigger( Trigger* trigger );
	void					addAction( Action* action );

	bool					getControlDuringAction() const { return m_bControlDuringAction; }
	void					setControlDuringAction(bool val) { m_bControlDuringAction = val; }


	friend class StartIncidentAction;
	
protected:
	virtual void			activate();
	virtual void			deactivate();

	TriggerList				m_trigger;
	ActionList				m_action;

private:
	bool					m_bControlDuringAction; // Indicates the capability of controlling player when actions in progress.
};

SCRIPT_FACTORY( SingleIncident )