#include "EmptyProjectPCH.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "FieldState.h"

IMPLEMENT_SINGLETON(WorldStateManager);

WorldStateManager::WorldStateManager(void)
{

}

WorldStateManager::~WorldStateManager(void)
{
}

void WorldStateManager::init()
{
	m_states = new State*[10];
	m_states[0] = new FieldState();
	m_states[1] = new BattleState();
	m_curStates = 0;
	m_nextState = m_states[0];
}

void WorldStateManager::release()
{
	m_states[0]->release();
	m_states[1]->release();
	delete m_states[0];
	delete m_states[1];
	delete [] m_states;
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
	}
}