#include "EmptyProjectPCH.h"
#include "CreditState.h"
#include "EpCamera.h"
#include "TopStateManager.h"

extern LPD3DXFONT						g_d3dxFont;			// Credit
extern LPD3DXFONT						g_d3dxFontBig;		// Credit


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

	throw std::runtime_error( "This method should not be called." );
	//return hr;
}

//////////////////////////////////////////////////////////////////////////

CreditState::CreditState( void )
{
	m_textMatAlpha		= 0;
	m_startTime			= -1.0f;
	m_textColor			= D3DXCOLOR( 0.8f, 0.9f, 0.7f, 0.0f );
	m_creditStartTime	= 0;
}

CreditState::~CreditState( void )
{
	release();
}

HRESULT CreditState::enter( double dStartTime )
{	
	ZeroMemory( &m_textMat, sizeof( D3DMATERIAL9 ) );
	m_textMat.Ambient.r = m_textMat.Ambient.g = m_textMat.Ambient.b = 0.9f;
	m_textMat.Specular.r = m_textMat.Specular.g = m_textMat.Specular.b = 1.0f;
	m_textMat.Diffuse.r = m_textMat.Diffuse.g = m_textMat.Diffuse.b = 1.0f;
	m_textMat.Ambient.a = m_textMat.Specular.a = m_textMat.Diffuse.a = 0.0f;
	m_textMat.Power = 1.0f;

	return State::enter( dStartTime );
}

HRESULT CreditState::leave()
{
	m_startTime = -1.0f;

	return S_OK;
}

HRESULT CreditState::frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	pd3dDevice->SetTransform( D3DTS_VIEW, (const D3DXMATRIX*)GetG().g_fixedViewMat.m );
	pd3dDevice->SetTransform( D3DTS_PROJECTION, (const D3DXMATRIX*)GetG().g_orthoProjMat.m );
	setupLight( pd3dDevice );

	RECT rc;
	rc.top = 20;
	rc.left = 20;
	rc.right = GetG().m_scrWidth - 20;
	rc.bottom = GetG().m_scrHeight - 20;

	double fStateTime = fTime - m_startTime;


	//StringCchPrintf( debugBuffer, 512, L"«««á«éªÎêÈöÇ: ( %.2f, %.2f, %.2f)", g_camera.GetEyePt()->x, g_camera.GetEyePt()->y, g_camera.GetEyePt()->z );
	

	if ( fStateTime < s_period * 1)
	{
		g_d3dxFont->DrawTextW( 0, L"Lead Programmer", -1, &rc, DT_NOCLIP | DT_RIGHT, m_textColor );
		rc.top += 25;
		g_d3dxFont->DrawTextW( 0, L"Young Shin Kim", -1, &rc, DT_NOCLIP | DT_RIGHT, m_textColor );
		rc.top -= 25;
	}
	else if ( fStateTime < s_period * 2)
	{
		g_d3dxFont->DrawTextW( 0, L"Yun Uk Eo", -1, &rc, DT_NOCLIP | DT_BOTTOM, m_textColor );
		rc.bottom -= 25;
		g_d3dxFont->DrawTextW( 0, L"User Interface Designer", -1, &rc, DT_NOCLIP | DT_BOTTOM, m_textColor );
		rc.bottom += 25;
	}
	else if ( fStateTime < s_period * 3)
	{
		g_d3dxFont->DrawTextW( 0, L"Battle System Programmer", -1, &rc, DT_NOCLIP, m_textColor );
		rc.top += 25;
		g_d3dxFont->DrawTextW( 0, L"Jae Woo Kim", -1, &rc, DT_NOCLIP, m_textColor );
		rc.top -= 25;
	}
	else if ( fStateTime < s_period * 4)
	{
		rc.top -= 20*2;
		g_d3dxFontBig->DrawTextW( 0, L"Sound Programmer", -1, &rc, DT_NOCLIP | DT_CENTER | DT_VCENTER, m_textColor );
		rc.top += 40*2;
		g_d3dxFontBig->DrawTextW( 0, L"Min Seok Baek", -1, &rc, DT_NOCLIP | DT_CENTER | DT_VCENTER, m_textColor );
	}
	else if ( fStateTime < s_period * 5)
	{
		g_d3dxFont->DrawTextW( 0, L"2008 PoolC Game Project", -1, &rc, DT_NOCLIP | DT_CENTER | DT_VCENTER, m_textColor );
	}

	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	ArnMatrix mTrans, mScale, mRot;
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

	return S_OK;
}

HRESULT CreditState::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == WM_KEYDOWN)
	{
		if ( wParam == VK_F4)
		{
			//TopStateManager::getSingleton().setNextState( GAME_TOP_STATE_PLAY );
		}
	}

	return S_OK;
	
}

HRESULT CreditState::release()
{
	return S_OK;
}

void CreditState::setupLight( IDirect3DDevice9* pd3dDevice ) 
{
	ArnLightData& light = GetG().m_light;

	ZeroMemory( &light, sizeof(ArnLightData) );
	light.Ambient.r = light.Ambient.g = light.Ambient.b = light.Ambient.a = 1.0f;
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = light.Diffuse.a = 1.0f;
	light.Direction = ArnVec3( 0.0f, 0.0f, 1.0f );
	light.Position = ArnVec3( 0.0f, 0.0f, -30.0f );
	light.Range = 1000.0f;
	light.Type = ARNLIGHT_DIRECTIONAL;

	pd3dDevice->SetLight( 0, (const D3DLIGHT9*)&light );
	pd3dDevice->LightEnable( 0, TRUE );

}

HRESULT CreditState::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	return S_OK;
}

void CreditState::onLostDevice()
{
}

HRESULT CreditState::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	return S_OK;
}