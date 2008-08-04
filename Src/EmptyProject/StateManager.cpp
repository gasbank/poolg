#include "EmptyProjectPCH.h"
#include "StateManager.h"
#include "IntroState.h"

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
	m_curStates = 0;
	m_nextState = m_states[0];
}

void StateManager::release()
{
	m_states[0]->release();
	delete m_states[0];
	delete [] m_states;
}

State* StateManager::getCurState() const
{
	return m_curStates;
}

void StateManager::transit()
{
	if (m_curStates != 0)
		m_curStates->leave();
	if (m_nextState == 0)
		throw std::runtime_error("Next State is NULL");
	m_curStates = m_nextState;
	m_curStates->enter();
}