#include "EmptyProjectPCH.h"
#include "BlockingActionIncident.h"
#include "Action.h"
#include "ScriptManager.h"

BlockingActionIncident::BlockingActionIncident( int trigCount )
: Incident( trigCount )
{
}

BlockingActionIncident::BlockingActionIncident( Trigger* trigger, Action* action, int trigCount )
: Incident( trigger, action, trigCount )
{
}

void BlockingActionIncident::activate()
{
	Incident::activate();

	m_curActionIt = m_action.begin();
}
bool BlockingActionIncident::update( double dTime, float fElapsedTime )
{
	if ( Incident::update( dTime, fElapsedTime ) == false )
		return false;

	assert( isActivated() );

	Action* curAction = (*m_curActionIt);
	bool curActionInProgress = curAction->update( dTime, fElapsedTime );
	if ( !curActionInProgress )
	{
		// Current action is finished. Try to activate next action if valid.
		++m_curActionIt;
		if ( m_curActionIt == m_action.end() )
		{
			m_LeastOnetime = true;
			deactivate();
			decreaseTrigCount();
			// All actions are finished.
			return true;
		}
		else
		{
			// m_curActionIt points to next action.
			if ( (*m_curActionIt)->isActivated() )
			{
				throw std::runtime_error( "Action planned to be invoked by Incident is already activated.\
										  You should deactivate the action when it is finished." );
			}
		}
	}

	// Some actions remained to be updated.
	return false;
}

//////////////////////////////////////////////////////////////////////////

Incident* EpCreateBlockingActionIncident( void* pv1, void* pv2, int trigCount )
{
	if ( trigCount == 0 )
		OutputDebugString( _T(" - EpWarn: BlockingActionIncident with triggering count 0 is created.\n" ) );

	Trigger* trig = reinterpret_cast<Trigger*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );
	return new BlockingActionIncident( trig, act, trigCount );
} SCRIPT_CALLABLE_PV_PV_PV_I( EpCreateBlockingActionIncident )

START_SCRIPT_FACTORY( BlockingActionIncident )
	CREATE_OBJ_COMMAND( EpCreateBlockingActionIncident )
END_SCRIPT_FACTORY( BlockingActionIncident )