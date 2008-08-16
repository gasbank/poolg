#include "EmptyProjectPCH.h"
#include "Incident.h"
#include "Trigger.h"
#include "Action.h"
#include "ScriptManager.h"

Incident::Incident( Trigger* trigger, Action* action )
: m_bActivated( false )
{
	TriggerList::iterator it1 = m_trigger.begin();
	m_trigger.push_back( trigger );

	ActionList::iterator it2 = m_action.begin();
	m_action.push_back( action );
}
Incident::~Incident(void)
{
}

bool Incident::update()
{
	int i = 0;

	if ( !m_bActivated )
	{
		TriggerList::iterator it1 = m_trigger.begin();
		for( ; it1 != m_trigger.end(); it1++ )
		{
			if ( (*it1)->check() )
				i++;
		}
		if ( i == (int) m_trigger.size() )
		{
			ActionList::iterator it2 = m_action.begin();
			for( ; it2 != m_action.end(); it2++ )
			{
				(*it2)->activate();
				
			}
			m_bActivated = true;
		}
	}

	ActionList::iterator it3 = m_action.begin();
	for( ; it3 != m_action.end(); it3++ )
		(*it3)->update();

	return true;
}

void Incident::release()
{
	EpSafeReleaseAll( m_trigger );
	EpSafeReleaseAll( m_action );
	
}

void Incident::addTrigger( Trigger* trigger )
{
	m_trigger.push_back( trigger );
}

void Incident::addAction( Action* action )
{
	m_action.push_back( action );
}


//////////////////////////////////////////////////////////////////////////

Incident* EpCreateIncident( void* pv1, void* pv2 )
{
	Trigger* trig = reinterpret_cast<Trigger*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );

	return new Incident( trig, act );
} SCRIPT_CALLABLE_PV_PV_PV( EpCreateIncident )

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
	CREATE_OBJ_COMMAND( EpAddTriggerToIncident )
	CREATE_OBJ_COMMAND( EpAddActionToIncident )
END_SCRIPT_FACTORY( Incident )
