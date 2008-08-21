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

	bool getLeastOnetime() { return m_LeastOnetime; }
	bool isFinished() const { return m_LeastOnetime; }

	void setName( const char* name ) { m_incidentName = name; }
	const char* getName() const { return m_incidentName.c_str(); }

	void printDebugInfo() const;
	void printDebugInfoDetailed() const;

protected:
	bool m_bActivated;
	TriggerList m_trigger;
	ActionList m_action;
	bool m_bInfinite;
	bool m_LeastOnetime;

private:
	std::string m_incidentName;

};

class BlockingActionIncident : public Incident
{
public:
	BlockingActionIncident( bool infinite );
	BlockingActionIncident( Trigger* trigger, Action* action, bool infinite );

	virtual bool update( double dTime, float fElapsedTime );
	//virtual bool isFinished() const { return ( m_curActionIt == m_action.end() ); }

private:
	ActionList::iterator m_curActionIt;
};

SCRIPT_FACTORY( Incident )