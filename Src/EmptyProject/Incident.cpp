#include "EmptyProjectPCH.h"
#include "Incident.h"
#include "Trigger.h"
#include "Action.h"
#include "ScriptManager.h"
#include "World.h"


Incident::Incident( bool infinite )
: m_bActivated( false ), m_bInfinite( infinite )
{
}

Incident::Incident( Trigger* trigger, Action* action, bool infinite )
: m_bActivated( false ), m_bInfinite( infinite )
{
	addTrigger( trigger );
	addAction( action );
}

Incident::~Incident(void)
{
}

bool Incident::update( double dTime, float fElapsedTime )
{
	int i = 0;

	if ( !m_bActivated )
	{
		TriggerList::iterator itTrig = m_trigger.begin();
		for( ; itTrig != m_trigger.end(); itTrig++ )
		{
			if ( (*itTrig)->check() )
				i++;
		}
		if ( i == (int) m_trigger.size() )
		{
			ActionList::iterator itAct = m_action.begin();
			for( ; itAct != m_action.end(); itAct++ )
			{
				(*itAct)->activate();
				
			}
			m_bActivated = true;
		}
	}

	if ( m_bActivated )
	{
		ActionList::iterator itAct = m_action.begin();
		for( ; itAct != m_action.end(); ++itAct )
			(*itAct)->update( dTime, fElapsedTime );
	}
	
	if ( m_bInfinite && ( GetWorldManager().getCurWorld()->getCurDialog() == 0 ) 
		&& (GetWorldManager().getCurWorld()->getHero()->getTilePos() != GetWorldManager().getCurWorld()->getHero()->getTileBufferPos()) )
		m_bActivated = false;

	return true;
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

//////////////////////////////////////////////////////////////////////////

BlockingActionIncident::BlockingActionIncident( bool infinite )
: Incident( infinite )
{
}

BlockingActionIncident::BlockingActionIncident( Trigger* trigger, Action* action, bool infinite )
: Incident( trigger, action, infinite )
{
}

bool BlockingActionIncident::update( double dTime, float fElapsedTime )
{
	int i = 0;
	if ( !m_bActivated )
	{
		TriggerList::iterator itTrig = m_trigger.begin();
		for( ; itTrig != m_trigger.end(); itTrig++ )
		{
			if ( (*itTrig)->check() )
				i++;
		}
	}
	
	if ( i == (int) m_trigger.size() )
	{
		assert( m_bActivated == false );
		
		// Should be called one time
		m_bActivated = true;
		m_curActionIt = m_action.begin();
		(*m_curActionIt)->activate();
	}

	if ( m_bActivated && ( m_curActionIt != m_action.end() ) )
	{
		if ( (*m_curActionIt)->update( dTime, fElapsedTime ) == false )
		{
			++m_curActionIt;
			if ( m_curActionIt == m_action.end() )
				return false;
			else
				(*m_curActionIt)->activate();
		}
		
	}
	return true;
}




//////////////////////////////////////////////////////////////////////////

Incident* EpCreateIncident( void* pv1, void* pv2, int b )
{
	Trigger* trig = reinterpret_cast<Trigger*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );

	bool infinite;
	if ( b == 0 )
		infinite = false;
	else
		infinite = true;

	return new Incident( trig, act, infinite );
} SCRIPT_CALLABLE_PV_PV_PV_I( EpCreateIncident )

Incident* EpCreateBlockingActionIncident( void* pv1, void* pv2, int b )
{
	Trigger* trig = reinterpret_cast<Trigger*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );
	return new BlockingActionIncident( trig, act, b?true:false );
} SCRIPT_CALLABLE_PV_PV_PV_I( EpCreateBlockingActionIncident )

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

START_SCRIPT_FACTORY( Incident )
	CREATE_OBJ_COMMAND( EpCreateIncident )
	CREATE_OBJ_COMMAND( EpCreateBlockingActionIncident )
	CREATE_OBJ_COMMAND( EpAddTriggerToIncident )
	CREATE_OBJ_COMMAND( EpAddActionToIncident )
END_SCRIPT_FACTORY( Incident )

//////////////////////////////////////////////////////////////////////////

