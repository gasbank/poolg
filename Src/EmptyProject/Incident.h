#pragma once

class Trigger;
class Action;
class StartIncidentAction;

typedef std::list<Trigger*> TriggerList;
typedef std::list<Action*> ActionList;

class Incident
{
public:
	friend class StartIncidentAction;

	Incident( int trigCount );
	Incident( Trigger* trigger, Action* action, int trigCount );
	virtual ~Incident(void);

	// Incident::update() returns false when there are some remaining
	// updates of action or one of triggers are not satisfied.
	// Returns true when all actions are finished.
	virtual bool update( double dTime, float fElapsedTime );
	virtual bool isFinished() const { return m_LeastOnetime; }

	void release();
	
	void addTrigger( Trigger* trigger );
	void addAction( Action* action );
	
	void detachAllTriggers();
	void detachAllActions();

	bool getLeastOnetime() { return m_LeastOnetime; }
	

	void setName( const char* name ) { m_incidentName = name; }
	const char* getName() const { return m_incidentName.c_str(); }

	void printDebugInfo() const;
	void printDebugInfoDetailed() const;

	
protected:
	virtual void activate();
	virtual void deactivate();

	bool checkTrigCountRemained() const { return ( m_trigCount == -1 || m_trigCount > 0 ); }
	void decreaseTrigCount() { if ( m_trigCount > 0 ) m_trigCount -= 1; }
	bool isActivated() const { return m_bActivated; }

	bool m_bActivated;
	TriggerList m_trigger;
	ActionList m_action;
	bool m_LeastOnetime;

	// Indicates the remaining occurrence count of this incident.
	// 0 means no more trigger testing(and no more action)
	// and -1 means infinite occurrence incident.
	int m_trigCount;

private:
	std::string m_incidentName;

};

class BlockingActionIncident : public Incident
{
public:
	BlockingActionIncident( int trigCount );
	BlockingActionIncident( Trigger* trigger, Action* action, int trigCount );

	virtual bool update( double dTime, float fElapsedTime );
	virtual bool isFinished() const { return ( m_LeastOnetime && !m_bActivated && m_curActionIt == m_action.end() ); }
protected:
	virtual void activate();
private:
	ActionList::iterator m_curActionIt;
};

SCRIPT_FACTORY( Incident )