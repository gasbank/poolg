#include "EmptyProjectPCH.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "FieldState.h"
#include "MenuState.h"

IMPLEMENT_SINGLETON(WorldStateManager);

WorldStateManager::WorldStateManager(void)
{
}

WorldStateManager::~WorldStateManager(void)
{
}

void WorldStateManager::init()
{
	m_states[GAME_WORLD_STATE_FIELD]	= new FieldState();
	m_states[GAME_WORLD_STATE_BATTLE]	= new BattleState();
	m_states[GAME_WORLD_STATE_MENU]		= new MenuState();
	m_curStates = 0;
	m_nextState = m_states[GAME_WORLD_STATE_FIELD];
}
