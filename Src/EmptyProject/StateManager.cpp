#include "EmptyProjectPCH.h"
#include "StateManager.h"
#include "State.h"

StateManager::StateManager(void)
{
}

StateManager::~StateManager(void)
{
}

State* StateManager::getCurState() const
{
	return m_curStates;
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
