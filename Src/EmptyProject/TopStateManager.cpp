#include "EmptyProjectPCH.h"
#include "TopStateManager.h"
#include "IntroState.h"
#include "WorldState.h"
#include "CreditState.h"
#include "ScriptManager.h"

IMPLEMENT_SINGLETON(TopStateManager);

TopStateManager::TopStateManager(void)
{
	m_states = new State*[TOTAL_STATE_COUNT];
	ZeroMemory( m_states, sizeof(State*) * TOTAL_STATE_COUNT );
	init();
}

TopStateManager::~TopStateManager(void)
{
	release();
	delete [] m_states;
}

void TopStateManager::init()
{	
	m_states[0] = new IntroState();
	m_states[2] = new WorldState();
	m_states[5] = new CreditState();

	m_curStates = 0;
	m_nextState = 0;
}

void TopStateManager::release()
{
	UINT i;
	for ( i = 0; i < TOTAL_STATE_COUNT; ++i )
	{
		EP_SAFE_RELEASE( m_states[ i ] );
	}	
}

void TopStateManager::setNextState(GameState state)
{
	switch (state)
	{
	case GAME_TOP_STATE_INTRO:
		m_nextState = m_states[0];
		break;
	case GAME_TOP_STATE_WORLD:
		m_nextState = m_states[2];
		break;
	case GAME_TOP_STATE_CREDIT:
		m_nextState = m_states[5];
		break;
	}

	// 다음 state가 현재 state와 같으면 state를 바꾸지 않는다.
	if (m_curStates == m_nextState)
		m_nextState = 0;
}

GameState TopStateManager::curStateEnum()
{
	if (m_curStates == m_states[0])
		return GAME_TOP_STATE_INTRO;
	else if (m_curStates == m_states[2])
		return GAME_TOP_STATE_WORLD;
	else if (m_curStates == m_states[5])
		return GAME_TOP_STATE_CREDIT;
	return NULL_STATE;
}

GameState TopStateManager::prevStateEnum()
{
	if (m_prevStates == m_states[0])
		return GAME_TOP_STATE_INTRO;
	else if (m_prevStates == m_states[2])
		return GAME_WORLD_STATE_BATTLE;
	else if (m_prevStates == m_states[5])
		return GAME_TOP_STATE_CREDIT;
	return NULL_STATE;
}

//////////////////////////////////////////////////////////////////////////



void* EpGetWorldState()
{
	return GetTopStateManager().getState(GAME_TOP_STATE_WORLD);
} SCRIPT_CALLABLE_PV( EpGetWorldState )

START_SCRIPT_FACTORY( TopStateManager )
	CREATE_OBJ_COMMAND( EpGetWorldState )
END_SCRIPT_FACTORY( TopStateManager )
