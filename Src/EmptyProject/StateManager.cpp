#include "EmptyProjectPCH.h"
#include "StateManager.h"
#include "State.h"

StateManager::StateManager(void)
{
}

StateManager::~StateManager(void)
{
}

void StateManager::setNextStateAsPrevState()
{
	m_nextState = m_prevStates;
}

State* StateManager::getCurState() const
{
	return m_curStates;
}

void StateManager::transit()
{
	if (m_nextState != 0)
	{
		if (m_curStates != 0)
			m_curStates->leave();
		
		m_prevStates = m_curStates;
		m_curStates = m_nextState;
		m_nextState = 0;
		m_curStates->enter();
	}
}