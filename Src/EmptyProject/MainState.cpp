#include "EmptyProjectPCH.h"
#include "MainState.h"
#include "World.h"
#include "WorldManager.h"
#include "TopStateManager.h"

MainState::MainState(void)
{
}

MainState::~MainState(void)
{
}

HRESULT MainState::enter( double dStartTime )
{
	return S_OK;
}

HRESULT MainState::leave()
{

	return S_OK;
}

HRESULT MainState::frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{
	getCurWorld()->frameRender( pd3dDevice, fTime, fElapsedTime );

	return S_OK;
}

HRESULT MainState::frameMove( double fTime, float fElapsedTime )
{
	getCurWorld()->frameMove( fTime, fElapsedTime );

	return S_OK;
}

HRESULT MainState::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == WM_KEYDOWN )
	{
		GetTopStateManager().setNextState( GAME_TOP_STATE_PLAY );
	}

	return S_OK;
}

HRESULT MainState::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{

	return S_OK;
}

HRESULT MainState::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{

	return S_OK;
}

void MainState::onLostDevice()
{

}

HRESULT MainState::release()
{

	return S_OK;
}