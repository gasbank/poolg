#pragma once
#include "Incident.h"

class Action;
class Trigger;
class BlockingActionIncident;

typedef std::list<BlockingActionIncident*> BlockingActionIncidentList;

class SequentialIncident : public Incident
{
public:
								SequentialIncident( int trigCount );
								~SequentialIncident();
	
	virtual bool				update( double dTime, float fElapsedTime );
	virtual bool				isFinished() const { return ( getLeastOnetime() && !isActivated() && m_curSequence == m_sequencer.end() ); }
	virtual void				printDebugInfo() const;
	virtual void				printDebugInfoDetailed() const;
	virtual void				release();

	void 						addTriggerSequence( Trigger* trigger );
	void 						addActionSequence( Action* action );
	
protected:
	virtual void				activate();
	virtual void				deactivate();

private:
	void						addIncident( BlockingActionIncident* incident ) { m_sequencer.push_back( incident ); }

	BlockingActionIncidentList				m_sequencer;
	BlockingActionIncident*					m_curIncident;
	BlockingActionIncidentList::iterator	m_curSequence;
	int										m_lastCheck;
};

SCRIPT_FACTORY( SequentialIncident )