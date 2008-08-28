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

HRESULT WorldStateManager::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	m_states[GAME_WORLD_STATE_FIELD]->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_WORLD_STATE_BATTLE]->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_WORLD_STATE_MENU]->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	return S_OK;
}

HRESULT WorldStateManager::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	m_states[GAME_WORLD_STATE_FIELD]->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_WORLD_STATE_BATTLE]->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_WORLD_STATE_MENU]->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	return S_OK;
}

void WorldStateManager::onLostDevice()
{
	/*m_states[GAME_WORLD_STATE_FIELD]->onLostDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_WORLD_STATE_BATTLE]->onLostDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_WORLD_STATE_MENU]->onLostDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );*/
}