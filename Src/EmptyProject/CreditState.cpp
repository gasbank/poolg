#include "EmptyProjectPCH.h"
#include "CreditState.h"
#include "EpCamera.h"

extern D3DXMATRIX				g_orthoProjMat;
extern D3DXMATRIX				g_fixedViewMat;
extern int						g_scrWidth;
extern int						g_scrHeight;

CreditState::CreditState(void)
{
	m_d3dxFont = 0;
	m_d3dxFontBig = 0;
}

CreditState::~CreditState(void)
{
	release();
}

void CreditState::enter()
{
	LPDIRECT3DDEVICE9& pd3dDevice = G::getSingleton().m_dev;
	EpCamera& camera = G::getSingleton().m_camera;
	
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

	D3DXCreateFont( pd3dDevice, 26, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Palatino Linotype"), &m_d3dxFont);
	D3DXCreateFont( pd3dDevice, 32, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Palatino Linotype"), &m_d3dxFontBig);

}

void CreditState::leave()
{
	
}

void CreditState::frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{
	WCHAR debugBuffer[512];
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = g_scrWidth;
	rc.bottom = g_scrHeight;

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
	
}

void CreditState::frameMove( double fTime, float fElapsedTime )
{

}

void CreditState::release()
{
	SAFE_RELEASE(m_d3dxFont);
	SAFE_RELEASE(m_d3dxFontBig);
}