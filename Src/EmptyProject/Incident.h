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
	virtual bool isFinished() const  = 0;

	void release();
	
	void addTrigger( Trigger* trigger );
	void addAction( Action* action );

	TriggerList getTriggerList() { return m_trigger; }
	ActionList getActionList() { return m_action; }
	
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
	
	TriggerList m_trigger;
	ActionList m_action;
	bool m_LeastOnetime;


private:
	std::string m_incidentName;
	bool m_bActivated;
	
	// Indicates the remaining occurrence count of this incident.
	// 0 means no more trigger testing(and no more action)
	// and -1 means infinite occurrence incident.
	int m_trigCount;
};


SCRIPT_FACTORY( Incident )	