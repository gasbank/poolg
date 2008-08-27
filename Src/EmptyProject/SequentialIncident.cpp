#include "EmptyProjectPCH.h"
#include "SequentialIncident.h"
#include "Trigger.h"
#include "Action.h"
#include "ScriptManager.h"
#include "BlockingActionIncident.h"

SequentialIncident::SequentialIncident( int trigCount )
: m_trigCount ( trigCount ), m_bActivated ( false )
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
		m_curIncident = new BlockingActionIncident( m_trigCount );
				
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
	if ( isActivated() )
		throw std::runtime_error( "Incident::activate() is called after prior activation" );
	m_bActivated = true;
}

void SequentialIncident::deactivate()
{
	if ( !isActivated() )
		throw std::runtime_error( "Incident::deactivate() is called during not activated state" );
	m_bActivated = false;
}

SequentialIncident::~SequentialIncident()
{

}

void SequentialIncident::release()
{
	EpSafeReleaseAll( m_sequencer );
}
//////////////////////////////////////////////////////////////////////////

int EpAddTriggerToSequence( void* pv1, void* pv2 )
{
	SequentialIncident* inci = reinterpret_cast<SequentialIncident*>( pv1 );
	Trigger* trig = reinterpret_cast<Trigger*>( pv2 );
	inci->addTriggerSequence( trig );

	return 0;
} SCRIPT_CALLABLE_I_PV_PV( EpAddTriggerToSequence )

int EpAddActionToSequence( void* pv1, void* pv2 )
{
	SequentialIncident* inci = reinterpret_cast<SequentialIncident*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );
	inci->addActionSequence( act );

	return 0;
} SCRIPT_CALLABLE_I_PV_PV( EpAddActionToSequence )

SequentialIncident* EpCreateSequentialIncident( int trigCount )
{
	if ( trigCount == 0 )
		OutputDebugString( _T(" - EpWarn: SequentialIncident with triggering count 0 is created.\n" ) );

	return new SequentialIncident( trigCount );
} SCRIPT_CALLABLE_PV_I( EpCreateSequentialIncident )

int EpSequentialIncidentSetName( void* pv1, const char* pv2 )
{
	SequentialIncident* inci = reinterpret_cast<SequentialIncident*>( pv1 );
	inci->setName( pv2 );
	return 0;
} SCRIPT_CALLABLE_I_PV_PC( EpSequentialIncidentSetName )

START_SCRIPT_FACTORY( SequentialIncident )
	CREATE_OBJ_COMMAND( EpAddTriggerToSequence )
	CREATE_OBJ_COMMAND( EpAddActionToSequence )
	CREATE_OBJ_COMMAND( EpCreateSequentialIncident )
	CREATE_OBJ_COMMAND( EpSequentialIncidentSetName )
END_SCRIPT_FACTORY( SequentialIncident )
