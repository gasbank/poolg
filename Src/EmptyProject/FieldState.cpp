#include "EmptyProjectPCH.h"
#include "FieldState.h"
#include "WorldStateManager.h"

FieldState::FieldState(void)
{
}

FieldState::~FieldState(void)
{
}

HRESULT FieldState::enter()
{
	return S_OK;
}

HRESULT FieldState::leave()
{
	return S_OK;
}

HRESULT FieldState::frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{	
	return S_OK;
}

HRESULT FieldState::frameMove( double fTime, float fElapsedTime )
{
	return S_OK;
}

HRESULT FieldState::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_F4)
		{
			WorldStateManager::getSingleton().setNextState(GAME_WORLD_STATE_BATTLE);
		}
	}

	return S_OK;
}

HRESULT FieldState::release()
{
	return S_OK;
}