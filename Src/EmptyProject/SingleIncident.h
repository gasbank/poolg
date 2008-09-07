#pragma once
#include "Incident.h"

class StartIncidentAction;



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