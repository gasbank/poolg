#include "EmptyProjectPCH.h"
#include "TopStateManager.h"
#include "IntroState.h"
#include "PlayState.h"
#include "CreditState.h"
#include "ScriptManager.h"

IMPLEMENT_SINGLETON(TopStateManager);

TopStateManager::TopStateManager(void)
{
}

TopStateManager::~TopStateManager(void)
{
}

void TopStateManager::init()
{
	m_states[GAME_TOP_STATE_INTRO]	= new IntroState();
	m_states[GAME_TOP_STATE_PLAY]	= new PlayState();
	m_states[GAME_TOP_STATE_CREDIT] = new CreditState();

	m_curStates = 0;
	m_nextState = 0;
}
