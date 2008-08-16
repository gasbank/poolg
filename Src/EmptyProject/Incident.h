#pragma once

class Trigger;
class Action;

typedef std::list<Trigger*> TriggerList;
typedef std::list<Action*> ActionList;

class Incident
{
public:
	Incident( Trigger* trigger, Action* action );
	~Incident(void);

	void release();
	bool update();
	void addTrigger( Trigger* trigger );
	void addAction( Action* action );
	
private:
	bool m_bActivated;
	TriggerList m_trigger;
	ActionList m_action;

};

SCRIPT_FACTORY( Incident )