#pragma once

class Trigger;
class Action;

typedef std::list<Trigger*> TriggerList;
typedef std::list<Action*> ActionList;

class Incident
{
public:
	Incident( Trigger* trigger, Action* action, bool infinite );
	~Incident(void);

	void release();
	bool update();
	void addTrigger( Trigger* trigger );
	void addAction( Action* action );
	
	void detachAllTriggers();
	void detachAllActions();

private:
	bool m_bActivated;
	TriggerList m_trigger;
	ActionList m_action;
	bool m_bInfinite;

};

SCRIPT_FACTORY( Incident )