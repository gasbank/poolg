#include "EmptyProjectPCH.h"
#include "FieldState.h"
#include "WorldStateManager.h"
#include "TopStateManager.h"
#include "WorldState.h"

FieldState::FieldState(void)
{
}

FieldState::~FieldState(void)
{
}

HRESULT FieldState::enter()
{
	EpCamera& camera = G::getSingleton().m_camera;

	m_vPrevEye = *camera.GetEyePt();
	m_vPrevLookAt = *camera.GetLookAtPt();

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

	EpCamera& camera = G::getSingleton().m_camera;

	// Get hero position
	TopStateManager& tsm = TopStateManager::getSingleton();
	WorldState* ws = static_cast<WorldState*>( tsm.getCurState() );
	const D3DXVECTOR3& vHeroPos = ws->getHeroPos();

	// Setup camera position.
	D3DXVECTOR3 vEye( vHeroPos.x, vHeroPos.y, vHeroPos.z - 30.0f );
	D3DXVECTOR3 vLookAt( vHeroPos );
	const D3DXVECTOR3 vUp( 0.0f, 1.0f, 0.0f );

	if (fStateTime < 1.0f && GetWorldStateManager().prevStateEnum() == GAME_WORLD_STATE_BATTLE)
	{
		D3DXVECTOR3 vPrevUp( 0.0f, 0.0f, -1.0f );

		D3DXVECTOR3 vCurEye;
		D3DXVECTOR3 vCurLookAt;
		D3DXVECTOR3 vCurUp;

		D3DXVec3Lerp( &vCurEye, &m_vPrevEye, &vEye, fStateTime );
		D3DXVec3Lerp( &vCurLookAt, &m_vPrevLookAt, &vLookAt, fStateTime );
		D3DXVec3Lerp( &vCurUp, &vPrevUp, &vUp, fStateTime );

		camera.SetViewParamsWithUp( &vCurEye, &vCurLookAt, vCurUp );
	} else {
		camera.SetViewParamsWithUp( &vEye, &vLookAt, vUp );
	}

	return S_OK;
}

HRESULT FieldState::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_F4)
		{
			GetWorldStateManager().setNextState(GAME_WORLD_STATE_BATTLE);
		}
	}

	return S_OK;
}

HRESULT FieldState::release()
{
	return S_OK;
}