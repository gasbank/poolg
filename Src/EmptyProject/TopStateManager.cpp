#include "EmptyProjectPCH.h"
#include "TopStateManager.h"
#include "IntroState.h"
#include "WorldState.h"
#include "CreditState.h"
#include "ScriptManager.h"

IMPLEMENT_SINGLETON(TopStateManager);

TopStateManager::TopStateManager(void)
{
	//m_states = new State*[TOTAL_STATE_COUNT]O;
	//ZeroMemory( m_states, sizeof(State*) * TOTAL_STATE_COUNT );
	init();
}

TopStateManager::~TopStateManager(void)
{
}

void TopStateManager::init()
{
	m_states[GAME_TOP_STATE_INTRO]	= new IntroState();
	m_states[GAME_TOP_STATE_WORLD]	= new WorldState();
	m_states[GAME_TOP_STATE_CREDIT] = new CreditState();

	m_curStates = 0;
	m_nextState = 0;
}


//////////////////////////////////////////////////////////////////////////



void* EpGetWorldState()
{
	return GetTopStateManager().getState(GAME_TOP_STATE_WORLD);
} SCRIPT_CALLABLE_PV( EpGetWorldState )

START_SCRIPT_FACTORY( TopStateManager )
	CREATE_OBJ_COMMAND( EpGetWorldState )
END_SCRIPT_FACTORY( TopStateManager )
