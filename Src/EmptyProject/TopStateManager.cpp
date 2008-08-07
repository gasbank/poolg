#include "EmptyProjectPCH.h"
#include "TopStateManager.h"
#include "IntroState.h"
#include "WorldState.h"
#include "CreditState.h"

IMPLEMENT_SINGLETON(TopStateManager);

TopStateManager::TopStateManager(void)
{
}

TopStateManager::~TopStateManager(void)
{
}

void TopStateManager::init()
{
	m_states = new State*[10];
	m_states[0] = new IntroState();
	m_states[2] = new WorldState();
	m_states[5] = new CreditState();
	m_curStates = 0;
	m_nextState = 0;
}

void TopStateManager::release()
{
	m_states[0]->release();
	m_states[2]->release();
	m_states[5]->release();
	delete m_states[0];
	delete m_states[2];
	delete m_states[5];
	delete [] m_states;
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