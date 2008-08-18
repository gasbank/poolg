#pragma once

class Trigger;
class Action;

typedef std::list<Trigger*> TriggerList;
typedef std::list<Action*> ActionList;

class Incident
{
public:
	Incident( bool infinite );
	Incident( Trigger* trigger, Action* action, bool infinite );
	virtual ~Incident(void);

	virtual bool update( double dTime, float fElapsedTime );

	void release();
	
	void addTrigger( Trigger* trigger );
	void addAction( Action* action );
	
	void detachAllTriggers();
	void detachAllActions();

protected:
	bool m_bActivated;
	TriggerList m_trigger;
	ActionList m_action;
	bool m_bInfinite;

};

class BlockingActionIncident : public Incident
{
public:
	BlockingActionIncident( bool infinite );
	BlockingActionIncident( Trigger* trigger, Action* action, bool infinite );

	virtual bool update( double dTime, float fElapsedTime );

private:
	ActionList::iterator m_curActionIt;
};

SCRIPT_FACTORY( Incident )