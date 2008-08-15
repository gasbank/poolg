#include "EmptyProjectPCH.h"
#include "Incident.h"
#include "Trigger.h"
#include "Action.h"
#include "ScriptManager.h"

Incident::Incident( Trigger* trigger, Action* action )
: m_bActivated( false )
{
	m_trigger = trigger;
	m_action = action;
}
Incident::~Incident(void)
{
}

bool Incident::update()
{
	if ( !m_bActivated && m_trigger->check() )
	{
		m_action->activate();
		m_bActivated = true;
	}

	m_action->update();

	return true;
}

void Incident::release()
{

}

//////////////////////////////////////////////////////////////////////////

Incident* EpCreateIncident( void* pv1, void* pv2 )
{
	Trigger* trig = reinterpret_cast<Trigger*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );

	return new Incident( trig, act );
} SCRIPT_CALLABLE_PV_PV_PV( EpCreateIncident )


START_SCRIPT_FACTORY( Incident )
	CREATE_OBJ_COMMAND( EpCreateIncident )
END_SCRIPT_FACTORY( Incident )
