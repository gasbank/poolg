#include "EmptyProjectPCH.h"
#include "StateManager.h"
#include "IntroState.h"
#include "WorldState.h"
#include "CreditState.h"

IMPLEMENT_SINGLETON(StateManager);

StateManager::StateManager(void)
{
}

StateManager::~StateManager(void)
{
}

void StateManager::init()
{
	m_states = new State*[100];
	m_states[0] = new IntroState();
	m_states[2] = new WorldState();
	m_states[5] = new CreditState();
	m_curStates = 0;
	m_nextState = 0;
}

void StateManager::release()
{
	m_states[0]->release();
	m_states[2]->release();
	m_states[5]->release();
	delete m_states[0];
	delete m_states[2];
	delete m_states[5];
	delete [] m_states;
}

State* StateManager::getCurState() const
{
	return m_curStates;
}

void StateManager::setNextState(GameTopState state)
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
}

void StateManager::transit()
{
	if (m_nextState)
	{
		if (m_curStates != 0)
			m_curStates->leave();
		
		m_curStates = m_nextState;
		m_nextState = 0;
		m_curStates->enter();
	}
}
