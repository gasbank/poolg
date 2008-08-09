#include "EmptyProjectPCH.h"
#include "CreditState.h"
#include "EpCamera.h"
#include "TopStateManager.h"
#include "Unit.h"

const float CreditState::s_period = 7.0f;
const float CreditState::s_fadeIn = 2.0f;
const float CreditState::s_fadeOut = 2.0f;

HRESULT createD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPCWSTR pStr, LPD3DXMESH* ppTextMesh, DWORD dwFontSize, LPCWSTR pStrFont, BOOL bBold = FALSE, BOOL bItalic = FALSE )
{
	HRESULT hr;
	LPD3DXMESH pMeshNew = NULL;
	HDC hdc = CreateCompatibleDC( NULL );
	if( hdc == NULL )
		return E_OUTOFMEMORY;
	INT nHeight = -MulDiv( dwFontSize, GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
	HFONT hFont;
	HFONT hFontOld;

	hFont = CreateFont( nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL,
		bItalic, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		pStrFont );

	hFontOld = ( HFONT )SelectObject( hdc, hFont );

	hr = D3DXCreateText( pd3dDevice, hdc, pStr, 0.01f, 0.2f, &pMeshNew, NULL, NULL );

	SelectObject( hdc, hFontOld );
	DeleteObject( hFont );
	DeleteDC( hdc );

	if ( SUCCEEDED( hr ) )
		*ppTextMesh = pMeshNew;

	return hr;
}

//////////////////////////////////////////////////////////////////////////

CreditState::CreditState( void )
{
	m_d3dxFont			= 0;
	m_d3dxFontBig		= 0;
	m_textMatAlpha		= 0;
	m_startTime			= -1.0f;
	m_textColor			= D3DXCOLOR( 0.8f, 0.9f, 0.7f, 0.0f );
	m_creditStartTime	= 0;
	m_text3D			= 0;
}

CreditState::~CreditState( void )
{
	release();
}

HRESULT CreditState::enter()
{
	LPDIRECT3DDEVICE9& pd3dDevice = GetG().m_dev;
	
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	pd3dDevice->SetTransform( D3DTS_VIEW, &GetG().g_fixedViewMat );
	pd3dDevice->SetTransform( D3DTS_PROJECTION, &GetG().g_orthoProjMat );

	D3DXCreateFont( pd3dDevice, 26, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T( "Palatino Linotype"), &m_d3dxFont );
	D3DXCreateFont( pd3dDevice, 32, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T( "Palatino Linotype"), &m_d3dxFontBig );

	setupLight();
	
	LPD3DXMESH text3D;
	createD3DXTextMesh( pd3dDevice, L"Hello World", &text3D, 100, L"Times New Roman" );
	m_text3D = Unit::createUnit( text3D );
	m_text3D->setUniformScale( 40.0f );
	m_text3D->setPosX( - 40.0f * ( m_text3D->getUpperRight().x - m_text3D->getLowerLeft().x ) / 2 );

	ZeroMemory( &m_textMat, sizeof( D3DMATERIAL9 ) );
	m_textMat.Ambient.r = m_textMat.Ambient.g = m_textMat.Ambient.b = 0.9f;
	m_textMat.Specular.r = m_textMat.Specular.g = m_textMat.Specular.b = 1.0f;
	m_textMat.Diffuse.r = m_textMat.Diffuse.g = m_textMat.Diffuse.b = 1.0f;
	m_textMat.Ambient.a = m_textMat.Specular.a = m_textMat.Diffuse.a = 0.0f;
	m_textMat.Power = 1.0f;

	return S_OK;
}

HRESULT CreditState::leave()
{
	m_startTime = -1.0f;

	return S_OK;
}

HRESULT CreditState::frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{
	RECT rc;
	rc.top = 20;
	rc.left = 20;
	rc.right = GetG().m_scrWidth - 20;
	rc.bottom = GetG().m_scrHeight - 20;

	double fStateTime = fTime - m_startTime;


	//StringCchPrintf( debugBuffer, 512, L"«««á«éªÎêÈöÇ: ( %.2f, %.2f, %.2f)", g_camera.GetEyePt()->x, g_camera.GetEyePt()->y, g_camera.GetEyePt()->z );
	

	if ( fStateTime < s_period * 1)
	{
		m_d3dxFont->DrawTextW( 0, L"Lead Programmer", -1, &rc, DT_NOCLIP | DT_RIGHT, m_textColor );
		rc.top += 25;
		m_d3dxFont->DrawTextW( 0, L"Young Shin Kim", -1, &rc, DT_NOCLIP | DT_RIGHT, m_textColor );
		rc.top -= 25;
	}
	else if ( fStateTime < s_period * 2)
	{
		m_d3dxFont->DrawTextW( 0, L"Yun Uk Eo", -1, &rc, DT_NOCLIP | DT_BOTTOM, m_textColor );
		rc.bottom -= 25;
		m_d3dxFont->DrawTextW( 0, L"User Interface Designer", -1, &rc, DT_NOCLIP | DT_BOTTOM, m_textColor );
		rc.bottom += 25;
	}
	else if ( fStateTime < s_period * 3)
	{
		m_d3dxFont->DrawTextW( 0, L"Battle System Programmer", -1, &rc, DT_NOCLIP, m_textColor );
		rc.top += 25;
		m_d3dxFont->DrawTextW( 0, L"Jae Woo Kim", -1, &rc, DT_NOCLIP, m_textColor );
		rc.top -= 25;
	}
	else if ( fStateTime < s_period * 4)
	{
		rc.top -= 20*2;
		m_d3dxFontBig->DrawTextW( 0, L"Sound Programmer", -1, &rc, DT_NOCLIP | DT_CENTER | DT_VCENTER, m_textColor );
		rc.top += 40*2;
		m_d3dxFontBig->DrawTextW( 0, L"Min Seok Baek", -1, &rc, DT_NOCLIP | DT_CENTER | DT_VCENTER, m_textColor );
	}

	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	D3DXMATRIX mTrans, mScale, mRot;
	D3DPERF_BeginEvent( 0xff00ff00, L"3D Text" );
	//m_text3D.draw();
	D3DPERF_EndEvent();
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	return S_OK;
}

HRESULT CreditState::frameMove( double fTime, float fElapsedTime )
{

	// Remember starting time
	if ( m_startTime <= 0.0f)
		m_startTime = fTime;

	float fStateTime = (float)( fTime - m_startTime );
	m_textMat.Ambient.a = m_textMat.Specular.a = m_textMat.Diffuse.a = m_textMatAlpha;
	m_textColor.a = m_textMatAlpha;

	if ( fStateTime >= m_creditStartTime && fStateTime < m_creditStartTime + s_fadeIn )
		m_textMatAlpha = (float)( ( fStateTime - m_creditStartTime) / s_fadeIn );
	else if ( fStateTime >= m_creditStartTime + s_period - s_fadeOut && fStateTime < m_creditStartTime + s_period )
		m_textMatAlpha = (float)( 1.0f - ( ( fStateTime - ( m_creditStartTime + s_period - s_fadeOut))) / s_fadeOut );
	else if ( fStateTime >= m_creditStartTime + s_period )
		m_creditStartTime = fStateTime;

	if ( m_textMatAlpha < 0 )
		m_textMatAlpha = 0;

	m_text3D->frameMove( fElapsedTime );

	if ( fStateTime >= s_period * 4)
	{
		TopStateManager::getSingleton().setNextState( GAME_TOP_STATE_WORLD );
	}
	return S_OK;
}

HRESULT CreditState::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == WM_KEYDOWN)
	{
		if ( wParam == VK_F4)
		{
			TopStateManager::getSingleton().setNextState( GAME_TOP_STATE_WORLD );
		}
	}

	return S_OK;
	
}

HRESULT CreditState::release()
{
	SAFE_RELEASE( m_d3dxFont );
	SAFE_RELEASE( m_d3dxFontBig );
	EP_SAFE_RELEASE(m_text3D);
	return S_OK;
}

void CreditState::setupLight() 
{
	D3DLIGHT9& light = GetG().m_light;
	LPDIRECT3DDEVICE9& pd3dDevice = GetG().m_dev;

	ZeroMemory( &light, sizeof( D3DLIGHT9) );
	light.Ambient.r = light.Ambient.g = light.Ambient.b = light.Ambient.a = 1.0f;
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = light.Diffuse.a = 1.0f;
	light.Direction = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	light.Position = D3DXVECTOR3( 0.0f, 0.0f, -30.0f );
	light.Range = 1000.0f;
	light.Type = D3DLIGHT_DIRECTIONAL;

	pd3dDevice->SetLight( 0, &light );
	pd3dDevice->LightEnable( 0, TRUE );

}
