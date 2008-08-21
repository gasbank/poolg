#include "EmptyProjectPCH.h"
#include "Incident.h"
#include "Trigger.h"
#include "Action.h"
#include "ScriptManager.h"
#include "World.h"


Incident::Incident( int trigCount )
: m_bActivated( false ), m_trigCount( trigCount ), m_LeastOnetime( false )
{
}

Incident::Incident( Trigger* trigger, Action* action, int trigCount )
: m_bActivated( false ), m_trigCount( trigCount ), m_LeastOnetime( false )
{
	addTrigger( trigger );
	addAction( action );
}

Incident::~Incident(void)
{
}


void Incident::release()
{
	EpSafeReleaseAll( m_trigger );
	EpSafeReleaseAll( m_action );
	
}

void Incident::addTrigger( Trigger* trigger )
{
	if ( !m_bActivated )
	{
		if ( trigger )
			m_trigger.push_back( trigger );
		else
			OutputDebugString( _T( " - EpWarn: Null 'Trigger' pointer will be ignored.\n" ) );
	}
	else
		throw std::runtime_error( "You cannot add any trigger after the incident activated" );
}

void Incident::addAction( Action* action )
{
	if ( !m_bActivated )
	{
		if ( action )
			m_action.push_back( action );
		else
			OutputDebugString( _T( " - EpWarn: Null 'Action' pointer will be ignored.\n" ) );
	}
	else
		throw std::runtime_error( "You cannot add any action after the incident activated" );
}

void Incident::printDebugInfo() const
{
	printf("Inc: %-30s #trig=%d #act=%d activated=%d trigCount=%d leastOnetime=%d\n", getName(), m_trigger.size(), m_action.size(), m_bActivated, m_trigCount, m_LeastOnetime );
}

void Incident::printDebugInfoDetailed() const
{
	printDebugInfo();
	EpPrintDebugInfoAll( m_trigger );
	EpPrintDebugInfoAll( m_action );
}

void Incident::activate()
{
	if ( isActivated() )
		throw std::runtime_error( "Incident::activate() is called after prior activation" );
	m_bActivated = true;

	if ( (*m_action.begin())->isActivated() )
	{
		throw std::runtime_error( "Action invoked by Incident is already activated. You should deactivate the action when it is finished." );
	}
}

void Incident::deactivate()
{
	if ( !isActivated() )
		throw std::runtime_error( "Incident::deactivate() is called during not activated state" );
	m_bActivated = false;
}

bool Incident::update( double dTime, float fElapsedTime )
{
	assert( m_trigger.size() );
	assert( m_action.size() );

	if ( !checkTrigCountRemained() )
		return false;

	if ( !isActivated() )
	{
		// The first action is not triggered yet.
		// Check each condition of triggers.
		int satisCount = 0;
		TriggerList::iterator itTrig = m_trigger.begin();
		for( ; itTrig != m_trigger.end(); itTrig++ )
		{
			if ( (*itTrig)->check() )
			{
				satisCount++;
			}
			else
			{
				// One of the trigger conditions is not satisfied. Do nothing.
				return false;
			}
		}

		// All triggers satisfy its condition.
		assert( satisCount == (int) m_trigger.size() );
		activate();
	}

	// All triggers are satisfied.
	return true;
}

//////////////////////////////////////////////////////////////////////////

int EpAddTriggerToIncident( void* pv1, void* pv2 )
{
	Incident* inci = reinterpret_cast<Incident*>( pv1 );
	Trigger* trig = reinterpret_cast<Trigger*>( pv2 );
	inci->addTrigger( trig );

	return 0;
} SCRIPT_CALLABLE_I_PV_PV( EpAddTriggerToIncident )

int EpAddActionToIncident( void* pv1, void* pv2 )
{
	Incident* inci = reinterpret_cast<Incident*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );
	inci->addAction( act );

	return 0;
} SCRIPT_CALLABLE_I_PV_PV( EpAddActionToIncident )

int EpIncidentSetName( void* pv1, const char* pv2 )
{
	Incident* inci = reinterpret_cast<Incident*>( pv1 );
	inci->setName( pv2 );
	return 0;
} SCRIPT_CALLABLE_I_PV_PC( EpIncidentSetName )


START_SCRIPT_FACTORY( Incident )
	CREATE_OBJ_COMMAND( EpIncidentSetName )
	CREATE_OBJ_COMMAND( EpAddTriggerToIncident )
	CREATE_OBJ_COMMAND( EpAddActionToIncident )
END_SCRIPT_FACTORY( Incident )
