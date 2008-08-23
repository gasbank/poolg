#pragma once

#include "BlockingActionIncident.h"

class Action;
class Trigger;

typedef std::list<BlockingActionIncident*> SequenceList;

class SequentialIncident
{
public:
	SequentialIncident( int trigCount );
	~SequentialIncident();

	void addTriggerSequence( Trigger* trigger );
	void addActionSequence( Action* action );
	void addIncident( BlockingActionIncident* incident ) { m_sequencer.push_back( incident ); }

	bool update( double dTime, float fElapsedTime );
	bool isFinished() { return ( m_LeastOnetime && !isActivated() && m_curSequence == m_sequencer.end() ); }

	bool isActivated() const { return m_bActivated; }

	void setName( const char* name ) { m_incidentName = name; }
	const char* getName() const { return m_incidentName.c_str(); }
protected:
	void activate();
	void deactivate();
private:
	bool checkTrigCountRemained() const { return ( m_trigCount == -1 || m_trigCount > 0 ); }
	void decreaseTrigCount() { if ( m_trigCount > 0 ) m_trigCount -= 1; }

	SequenceList m_sequencer;
	BlockingActionIncident* m_curIncident;
	SequenceList::iterator m_curSequence;
	bool m_bActivated;
	bool m_LeastOnetime;
	int m_trigCount;
	int m_lastCheck;
	std::string m_incidentName;
};

SCRIPT_FACTORY( SequentialIncident )