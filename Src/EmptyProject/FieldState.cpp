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

	if (fStateTime < 1.0f)
	{
		TopStateManager& tsm = TopStateManager::getSingleton();
		WorldState* ws = static_cast<WorldState*>( tsm.getCurState() );
		const D3DXVECTOR3& vHeroPos = ws->getHeroPos();

		EpCamera& camera = G::getSingleton().m_camera;

		D3DXVECTOR3 vLookEye( 0.0f, 0.0f, -30.0f );
		D3DXVECTOR3 vLookAt( vHeroPos );
		const D3DXVECTOR3 vUp( 0.0f, 1.0f, 0.0f );

		D3DXVECTOR3 vPrevUp( 0.0f, 0.0f, -1.0f );

		D3DXVECTOR3 vCurEye;
		D3DXVECTOR3 vCurLookAt;
		D3DXVECTOR3 vCurUp;

		D3DXVec3Lerp( &vCurEye, &m_vPrevEye, &vLookEye, fStateTime );
		D3DXVec3Lerp( &vCurLookAt, &m_vPrevLookAt, &vLookAt, fStateTime );
		D3DXVec3Lerp( &vCurUp, &vPrevUp, &vUp, fStateTime );

		camera.SetViewParamsWithUp( &vCurEye, &vCurLookAt, vCurUp );
	}

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