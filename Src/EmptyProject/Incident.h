#pragma once

class Trigger;
class Action;

class Incident
{
public:
	Incident( Trigger* trigger, Action* action );
	~Incident(void);

	void release();
	bool update();
	
private:
	bool m_bActivated;
	Trigger* m_trigger;
	Action* m_action;

};
