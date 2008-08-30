#include "EmptyProjectPCH.h"
#include "SingleIncident.h"
#include "World.h"
#include "Trigger.h"
#include "Action.h"
#include "ScriptManager.h"

SingleIncident::SingleIncident( int trigCount )
: Incident( trigCount ), m_bControlDuringAction( false )
{
}

SingleIncident::SingleIncident( Trigger* trigger, Action* action, int trigCount )
: Incident( trigCount ), m_bControlDuringAction( false )
{
	addTrigger( trigger );
	addAction( action );
}

SingleIncident::~SingleIncident(void)
{
}


bool SingleIncident::update( double dTime, float fElapsedTime )
{
	assert( !m_trigger.empty() );
	assert( !m_action.empty() );

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



void SingleIncident::activate()
{
	Incident::activate();

	if ( (*m_action.begin())->isActivated() )
	{
		throw std::runtime_error( "Action invoked by Incident is already activated. You should deactivate the action when it is finished." );
	}


	GetWorldManager().getCurWorld()->getHeroUnit()->setControllable( m_bControlDuringAction );
}

void SingleIncident::deactivate()
{
	Incident::deactivate();

	GetWorldManager().getCurWorld()->getHeroUnit()->setControllable( !m_bControlDuringAction );
}

void SingleIncident::release()
{
	EpSafeReleaseAll( m_trigger );
	EpSafeReleaseAll( m_action );

	Incident::release();
}


void SingleIncident::printDebugInfo() const
{
	printf("Inc: %-30s #trig=%d #act=%d activated=%d trigCount=%d leastOnetime=%d\n", getName(), m_trigger.size(), m_action.size(), isActivated(), getTrigCount(), getLeastOnetime() );
}

void SingleIncident::printDebugInfoDetailed() const
{
	printDebugInfo();
	EpPrintDebugInfoAll( m_trigger );
	EpPrintDebugInfoAll( m_action );
}

void SingleIncident::addTrigger( Trigger* trigger )
{
	if ( !isActivated() )
	{
		if ( trigger )
			m_trigger.push_back( trigger );
		else
			OutputDebugString( _T( " - EpWarn: Null 'Trigger' pointer will be ignored.\n" ) );
	}
	else
		throw std::runtime_error( "You cannot add any trigger after the incident activated" );
}

void SingleIncident::addAction( Action* action )
{
	if ( !isActivated() )
	{
		if ( action )
			m_action.push_back( action );
		else
			OutputDebugString( _T( " - EpWarn: Null 'Action' pointer will be ignored.\n" ) );
	}
	else
		throw std::runtime_error( "You cannot add any action after the incident activated" );
}

//////////////////////////////////////////////////////////////////////////

int EpSingleIncidentAddTrigger( void* pv1, void* pv2 )
{
	SingleIncident* inci = reinterpret_cast<SingleIncident*>( pv1 );
	Trigger* trig = reinterpret_cast<Trigger*>( pv2 );
	inci->addTrigger( trig );

	return 0;
} SCRIPT_CALLABLE_I_PV_PV( EpSingleIncidentAddTrigger )

int EpSingleIncidentAddAction( void* pv1, void* pv2 )
{
	SingleIncident* inci = reinterpret_cast<SingleIncident*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );
	inci->addAction( act );
	return 0;
} SCRIPT_CALLABLE_I_PV_PV( EpSingleIncidentAddAction )


int EpSingleIncidentSetControlDuringAction( void* pv1, int b )
{
	SingleIncident* inci = reinterpret_cast<SingleIncident*>( pv1 );
	inci->setControlDuringAction( b?true:false );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpSingleIncidentSetControlDuringAction )


START_SCRIPT_FACTORY( SingleIncident )
	CREATE_OBJ_COMMAND_NEWNAME( EpSingleIncidentAddTrigger,				"EpAddTriggerToIncident" )
	CREATE_OBJ_COMMAND_NEWNAME( EpSingleIncidentAddAction,				"EpAddActionToIncident" )
	CREATE_OBJ_COMMAND_NEWNAME( EpSingleIncidentSetControlDuringAction, "EpIncidentSetControlDuringAction" )
END_SCRIPT_FACTORY( SingleIncident )