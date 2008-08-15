#include "EmptyProjectPCH.h"
#include "Incident.h"
#include "Trigger.h"
#include "Action.h"

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