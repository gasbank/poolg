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

HRESULT TopStateManager::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	m_states[GAME_TOP_STATE_INTRO]->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_TOP_STATE_PLAY]->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_TOP_STATE_CREDIT]->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	return S_OK;
}

void TopStateManager::onLostDevice()
{
}

HRESULT TopStateManager::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	m_states[GAME_TOP_STATE_INTRO]->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_TOP_STATE_PLAY]->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	m_states[GAME_TOP_STATE_CREDIT]->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	return S_OK;
}
