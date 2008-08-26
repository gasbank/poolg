#include "EmptyProjectPCH.h"
#include "FieldState.h"
#include "WorldStateManager.h"
#include "World.h"
#include "WorldManager.h"
#include "TopStateManager.h"
#include "World.h"

FieldState::FieldState(void)
{
	m_bCamManualMovement = false;
}

FieldState::~FieldState(void)
{
}

HRESULT FieldState::enter()
{
	setupCamera();

	return S_OK;
}

HRESULT FieldState::leave()
{
	m_startTime = -1.0f;

	return S_OK;
}

HRESULT FieldState::frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{	
	return S_OK;
}

HRESULT FieldState::frameMove( double fTime, float fElapsedTime )
{
	float fStateTime = (float)getStateTime(fTime);

	return S_OK;
}

HRESULT FieldState::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (uMsg == WM_KEYDOWN)
	{
		if ( wParam == VK_F6 )
		{
			m_bCamManualMovement = !m_bCamManualMovement;
		}
	}

	return S_OK;
}

HRESULT FieldState::release()
{
	return S_OK;
}

void FieldState::setupCamera()
{
	EpCamera& camera = G::getSingleton().m_camera;

	// Get hero position
	const D3DXVECTOR3& vHeroPos = getCurWorld()->getHeroPos();

	camera.setAttachPos( &vHeroPos );
	camera.setSmoothCameraDuration( 1.0f );
	camera.begin( CAMERA_SMOOTH_ATTACH );
}