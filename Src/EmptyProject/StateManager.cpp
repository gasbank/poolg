#include "EmptyProjectPCH.h"
#include "StateManager.h"
#include "State.h"

StateManager::StateManager(void)
{
}

StateManager::~StateManager( void )
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

State* StateManager::getState( GameState gs )
{
	return m_states[gs];
}

void StateManager::release()
{
	StateMap::iterator it = m_states.begin();
	for ( ; it != m_states.end(); ++it )
	{
		EP_SAFE_RELEASE( it->second );
	}
}


void StateManager::setNextState(GameState state)
{
	if ( m_states.find( state ) != m_states.end() )
	{
		if ( m_curStates != m_states[state] )
			m_nextState = m_states[state];
	}
	else
	{
		throw std::runtime_error( "Invalid GameState enum" );
	}
	
}

void StateManager::setNextStateForced( GameState state )
{
	if ( m_states.find( state ) != m_states.end() )
		m_nextState = m_states[state];
	else
		throw std::runtime_error( "Invalid GameState enum" );
}

GameState StateManager::curStateEnum()
{
	StateMap::iterator it = m_states.begin();
	for ( ; it != m_states.end(); ++it )
	{
		if ( it->second == m_curStates )
			return it->first;
	}
	return NULL_STATE;
}

GameState StateManager::prevStateEnum()
{
	StateMap::iterator it = m_states.begin();
	for ( ; it != m_states.end(); ++it )
	{
		if ( it->second == m_prevStates )
			return it->first;
	}
	return NULL_STATE;
}