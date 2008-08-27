#include "EmptyProjectPCH.h"
#include "NonblockingActionIncident.h"
#include "Action.h"
#include "ScriptManager.h"

NonblockingActionIncident::NonblockingActionIncident( int trigCount )
: SingleIncident( trigCount )
{
}

NonblockingActionIncident::NonblockingActionIncident( Trigger* trigger, Action* action, int trigCount )
: SingleIncident( trigger, action, trigCount )
{
}

bool NonblockingActionIncident::update( double dTime, float fElapsedTime )
{
	if ( SingleIncident::update( dTime, fElapsedTime ) == false )
		return false;

	assert( isActivated() );

	ActionList::iterator itAct = m_action.begin();

	UINT finishedActionCount = 0;
	for( ; itAct != m_action.end(); ++itAct )
	{
		bool updateIsNeededNextTime = (*itAct)->update( dTime, fElapsedTime );
		if( !updateIsNeededNextTime )
		{
			// Current action is finished.
			finishedActionCount++;
		}
	}
	if ( finishedActionCount == m_action.size() )
	{
		deactivate();
		decreaseTrigCount();
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

Incident* EpCreateNonblockingActionIncident( void* pv1, void* pv2, int trigCount )
{
	if ( trigCount == 0 )
		OutputDebugString( _T(" - EpWarn: NonblockingActionIncident with triggering count 0 is created.\n" ) );

	Trigger* trig = reinterpret_cast<Trigger*>( pv1 );
	Action* act = reinterpret_cast<Action*>( pv2 );
	return new NonblockingActionIncident( trig, act, trigCount );
} SCRIPT_CALLABLE_PV_PV_PV_I( EpCreateNonblockingActionIncident )


START_SCRIPT_FACTORY( NonblockingActionIncident )
	CREATE_OBJ_COMMAND( EpCreateNonblockingActionIncident )
END_SCRIPT_FACTORY( NonblockingActionIncident )