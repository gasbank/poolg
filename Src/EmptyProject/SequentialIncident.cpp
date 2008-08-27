#include "EmptyProjectPCH.h"
#include "SequentialIncident.h"
#include "Trigger.h"
#include "Action.h"
#include "ScriptManager.h"
#include "BlockingActionIncident.h"

SequentialIncident::SequentialIncident( int trigCount )
: Incident( trigCount )
{
	m_curIncident = 0;
	m_lastCheck = 1;
}

bool SequentialIncident::update( double dTime, float fElapsedTime )
{
	if ( !checkTrigCountRemained() )
		return false;

	if ( !isActivated() )
	{
		if ( m_curSequence != m_sequencer.end() )
			if ( !(*m_curSequence)->update( dTime, fElapsedTime ) )
				return false;
		activate();
		return true;
	}
	else if ( m_curSequence != m_sequencer.end() )
	{
		if ( (*m_curSequence)->isFinished() )
		{
			deactivate();
			m_curSequence++;
		}
	}

	if ( isFinished() )
	{
		decreaseTrigCount();
		m_curSequence = m_sequencer.begin();
	}

	return false;
}

void SequentialIncident::addTriggerSequence( Trigger* trigger )
{
	if ( m_lastCheck == 1 )
		m_curIncident = new BlockingActionIncident( getTrigCount() );
				
	m_curIncident->addTrigger( trigger );
	m_lastCheck = 0;
}

void SequentialIncident::addActionSequence( Action* action )
{
	if ( m_lastCheck == 0 )
	{
		addIncident( m_curIncident );
		m_curSequence = m_sequencer.begin();
	}

	m_curIncident->addAction( action );
	m_lastCheck = 1;
}

void SequentialIncident::activate()
{
	Incident::activate();
}

void SequentialIncident::deactivate()
{
	Incident::deactivate();
}

SequentialIncident::~SequentialIncident()
{

}

void SequentialIncident::release()
{
	EpSafeReleaseAll( m_sequencer );

	Incident::release();
}

void SequentialIncident::printDebugInfo() const
{
	throw std::runtime_error( "Not implemented yet" );
}

void SequentialIncident::printDebugInfoDetailed() const
{
	throw std::runtime_error( "Not implemented yet" );
}

//////////////////////////////////////////////////////////////////////////

int EpSequentialIncidentAddTrigger( void* pv1, void* pv2 )
{
	SequentialIncident* inci = reinterpret_cast<SequentialIncident*>( pv1 );
	Trigger* trig = reinterpret_cast<Trigger*>( pv2 );
	inci->addTriggerSequence( trig );

	return 0;
} SCRIPT_CALLABLE_I_PV_PV( EpSequentialIncidentAddTrigger )

int EpSequentialIncidentAddAction( void* pv1, void* pv2 )
{
	SequentialIncident* inci = reinterpret_cast<SequentialIncident*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );
	inci->addActionSequence( act );

	return 0;
} SCRIPT_CALLABLE_I_PV_PV( EpSequentialIncidentAddAction )

SequentialIncident* EpCreateSequentialIncident( int trigCount )
{
	if ( trigCount == 0 )
		OutputDebugString( _T(" - EpWarn: SequentialIncident with triggering count 0 is created.\n" ) );

	return new SequentialIncident( trigCount );
} SCRIPT_CALLABLE_PV_I( EpCreateSequentialIncident )


START_SCRIPT_FACTORY( SequentialIncident )
	CREATE_OBJ_COMMAND( EpCreateSequentialIncident )
	CREATE_OBJ_COMMAND_NEWNAME( EpSequentialIncidentAddTrigger,		"EpAddTriggerToSequence" )
	CREATE_OBJ_COMMAND_NEWNAME( EpSequentialIncidentAddAction,		"EpAddActionToSequence" )
END_SCRIPT_FACTORY( SequentialIncident )
