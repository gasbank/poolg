#include "EmptyProjectPCH.h"
#include "CreditState.h"
#include "EpCamera.h"
#include "StateManager.h"

extern D3DXMATRIX				g_orthoProjMat;
extern D3DXMATRIX				g_fixedViewMat;

CreditState::CreditState(void)
{
	m_d3dxFont = 0;
	m_d3dxFontBig = 0;
	m_startTime = -1.0f;
}

CreditState::~CreditState(void)
{
	release();
}

HRESULT CreditState::enter()
{
	LPDIRECT3DDEVICE9& pd3dDevice = G::getSingleton().m_dev;
	EpCamera& camera = G::getSingleton().m_camera;
	
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

	D3DXCreateFont( pd3dDevice, 26, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Palatino Linotype"), &m_d3dxFont);
	D3DXCreateFont( pd3dDevice, 32, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Palatino Linotype"), &m_d3dxFontBig);

	setupLight();

	return S_OK;
}

HRESULT CreditState::leave()
{
	m_startTime = -1.0f;

	return S_OK;
}

HRESULT CreditState::frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{
	WCHAR debugBuffer[512];
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = G::getSingleton().m_scrWidth;
	rc.bottom = G::getSingleton().m_scrHeight;

	// Remember starting time
	if (m_startTime <= 0.0f)
		m_startTime = fTime;

	// Set fTime as elasped time from starting of state.
	fTime = fTime - m_startTime;

	//StringCchPrintf(debugBuffer, 512, L"«««á«éªÎêÈöÇ: (%.2f, %.2f, %.2f)", g_camera.GetEyePt()->x, g_camera.GetEyePt()->y, g_camera.GetEyePt()->z);
	if (fTime < 5.0f)
	{
		m_d3dxFont->DrawTextW(0, L"Lead Programmer", -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
		rc.top += 25;
		m_d3dxFont->DrawTextW(0, L"Young Shin Kim", -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
		rc.top -= 25;
	}
	else if (fTime < 10.0f)
	{
		m_d3dxFont->DrawTextW(0, L"Yun Uk Eo", -1, &rc, DT_NOCLIP | DT_BOTTOM, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
		rc.bottom -= 25;
		m_d3dxFont->DrawTextW(0, L"User Interface Designer", -1, &rc, DT_NOCLIP | DT_BOTTOM, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
		rc.bottom += 25;
	}
	else if (fTime < 15.0f)
	{
		m_d3dxFont->DrawTextW(0, L"Battle System Programmer", -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
		rc.top += 25;
		m_d3dxFont->DrawTextW(0, L"Jae Woo Kim", -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
		rc.top -= 25;
	}
	else if (fTime < 20.0f)
	{
		rc.top -= 25*2;
		m_d3dxFontBig->DrawTextW(0, L"Sound Programmer", -1, &rc, DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
		rc.top += 50*2;
		m_d3dxFontBig->DrawTextW(0, L"Min Seok Baek", -1, &rc, DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	}

	return S_OK;
}

HRESULT CreditState::frameMove( double fTime, float fElapsedTime )
{
	return S_OK;
}

HRESULT CreditState::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_F4)
		{
			StateManager::getSingleton().setNextState(GAME_TOP_STATE_WORLD);
			StateManager::getSingleton().transit();
		}
	}

	return S_OK;
}

HRESULT CreditState::release()
{
	SAFE_RELEASE(m_d3dxFont);
	SAFE_RELEASE(m_d3dxFontBig);

	return S_OK;
}

void CreditState::setupLight() 
{
	D3DLIGHT9& light = G::getSingleton().m_light;
	LPDIRECT3DDEVICE9& pd3dDevice = G::getSingleton().m_dev;

	//ZeroMemory(&light, sizeof(D3DLIGHT9));
	//D3DCOLORVALUE cv = { 0.8f, 0.8f, 0.8f, 1.0f };
	//light.Ambient = cv;
	//light.Diffuse = cv;
	//light.Specular = cv;

	D3DXVECTOR3 dir(0.0f, 10.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &dir);

	D3DXVECTOR3 pos(-10.0f, 0.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Position, &pos);

	//// What are these?
	//light.Falloff = 0.5f; 
	//light.Phi = D3DXToRadian(80);
	//light.Theta = D3DXToRadian(10);

	//light.Type = D3DLIGHT_DIRECTIONAL;
	//light.Range = 1000.0f;

	//pd3dDevice->SetLight(0, &light);
	//pd3dDevice->LightEnable(0, TRUE);
}