#include "EmptyProjectPCH.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "FieldState.h"
#include "MenuState.h"

IMPLEMENT_SINGLETON(WorldStateManager);

WorldStateManager::WorldStateManager(void)
{
	m_states = new State*[TOTAL_STATE_COUNT];
	ZeroMemory( m_states, sizeof(State*) * TOTAL_STATE_COUNT );
	init();
}

WorldStateManager::~WorldStateManager(void)
{
	release();
	delete [] m_states;
}

void WorldStateManager::init()
{
	m_states[0] = new FieldState();
	m_states[1] = new BattleState();
	m_states[2] = new MenuState();
	m_curStates = 0;
	m_nextState = m_states[0];
}

void WorldStateManager::release()
{
	UINT i;
	for ( i = 0; i < TOTAL_STATE_COUNT; ++i )
	{
		EP_SAFE_RELEASE( m_states[ i ] );
	}
}

void WorldStateManager::setNextState(GameState state)
{
	switch (state)
	{
	case GAME_WORLD_STATE_FIELD:
		m_nextState = m_states[0];
		break;
	case GAME_WORLD_STATE_BATTLE:
		m_nextState = m_states[1];
		break;
	case GAME_WORLD_STATE_MENU:
		m_nextState = m_states[2];
		break;
	}

	// 다음 state가 현재 state와 같으면 state를 바꾸지 않는다.
	if (m_curStates == m_nextState)
		m_nextState = 0;
}

GameState WorldStateManager::curStateEnum()
{
	if (m_curStates == m_states[0])
		return GAME_WORLD_STATE_FIELD;
	else if (m_curStates == m_states[1])
		return GAME_WORLD_STATE_BATTLE;
	else if (m_curStates == m_states[2])
		return GAME_WORLD_STATE_MENU;
	return NULL_STATE;
}

GameState WorldStateManager::prevStateEnum()
{
	if (m_prevStates == m_states[0])
		return GAME_WORLD_STATE_FIELD;
	else if (m_prevStates == m_states[1])
		return GAME_WORLD_STATE_BATTLE;
	else if (m_prevStates == m_states[2])
		return GAME_WORLD_STATE_MENU;
	return NULL_STATE;
}