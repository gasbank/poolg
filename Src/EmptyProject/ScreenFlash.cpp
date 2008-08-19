#include "EmptyProjectPCH.h"
#include "ScreenFlash.h"

#include "ShaderWrapper.h"

ScreenFlash::ScreenFlash(void)
{
	m_alphaShader			= 0;
	m_testPolygon			= 0;
	m_testPolygonCloned		= 0;
	m_screenFlashAlphaAngle	= 180.0f;
}

ScreenFlash::~ScreenFlash(void)
{
}

void ScreenFlash::setup()
{
	LPDIRECT3DDEVICE9& pd3dDevice = GetG().m_dev;

	//////////////////////////////////////////////////////////////////////////
	// Prepare alpha shading
	m_alphaShader = new AlphaShader();
	m_alphaShader->initShader( pd3dDevice, L"Shaders/Alpha.vsh" );
	m_alphaShader->compileShader( "Alpha", "vs_2_0" );

	D3DXCreatePolygon( pd3dDevice, 10.0f, 32, &m_testPolygon, 0 );
	m_testPolygon->CloneMesh( D3DXMESH_WRITEONLY, m_alphaShader->getDecl(), pd3dDevice, &m_testPolygonCloned );
	//////////////////////////////////////////////////////////////////////////
}

HRESULT ScreenFlash::frameRender()
{
	//////////////////////////////////////////////////////////////////////////
	// Draw alpha animated plane

	HRESULT hr = S_OK;

	if ( !m_bStop )
	{
		GetG().m_dev->SetRenderState( D3DRS_LIGHTING, FALSE );
		GetG().m_dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		GetG().m_dev->SetTexture( 0, 0 );
		V_RETURN( GetG().m_dev->SetVertexShader( m_alphaShader->getVertexShader() ) );
		D3DPERF_BeginEvent( 0, L"Draw Alpha Animated" );
		V_RETURN( m_testPolygonCloned->DrawSubset( 0 ) );
		D3DPERF_EndEvent();
		V_RETURN( GetG().m_dev->SetVertexShader( 0 ) );
	}

	//////////////////////////////////////////////////////////////////////////

	return hr;
}


HRESULT ScreenFlash::frameMove( double fTime, float fElapsedTime )
{
	//////////////////////////////////////////////////////////////////////////
	// Alpha shading logic

	HRESULT hr = S_OK;

	D3DXMATRIXA16 mWorldViewProj;
	D3DXMatrixIdentity( &mWorldViewProj );
	V( m_alphaShader->getConstantTable()->SetMatrix( DXUTGetD3D9Device(), "mWorldViewProj", &mWorldViewProj ) );
	V( m_alphaShader->getConstantTable()->SetFloat( DXUTGetD3D9Device(), "fTime", (float)fTime ) );

	m_bStop = false;

	// Change alpha for duration m_redFadeDurationSec
	if ( m_screenFlashAlphaAngle < 90.0f  )
		m_screenFlashAlphaAngle += fElapsedTime * 180.0f / m_screenFlashDurationSec * 2.0f;
	else if ( m_screenFlashAlphaAngle < 180.0f  )
		m_screenFlashAlphaAngle += fElapsedTime * 180.0f / m_screenFlashDurationSec / 4.0f;
	else if ( m_screenFlashAlphaAngle != 180.0f )
		m_screenFlashAlphaAngle = 180.0f;
	else
		m_bStop = true;

	//printf("%f \n", m_screenFlashAlphaAngle);

	V( m_alphaShader->getConstantTable()->SetFloat( 
		DXUTGetD3D9Device(), 
		"alpha", 
		abs( sin( D3DXToRadian( m_screenFlashAlphaAngle ) ) / 2 ) ) );
	V( m_alphaShader->getConstantTable()->SetFloat( DXUTGetD3D9Device(), "diffuseR", m_screenFlashCV.r ) );
	V( m_alphaShader->getConstantTable()->SetFloat( DXUTGetD3D9Device(), "diffuseG", m_screenFlashCV.g ) );
	V( m_alphaShader->getConstantTable()->SetFloat( DXUTGetD3D9Device(), "diffuseB", m_screenFlashCV.b ) );

	//////////////////////////////////////////////////////////////////////////
	
	return hr;
}

void ScreenFlash::release()
{
	SAFE_RELEASE( m_testPolygon );
	SAFE_RELEASE( m_testPolygonCloned );
	EP_SAFE_RELEASE( m_alphaShader );
}

HRESULT ScreenFlash::handleMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_KEYDOWN:
		switch ( wParam )
		{
		case 'H':
			screenFlashing( 0.3f, 1.0f, 0.0f, 0.0f );
			break;
		}
		break;
	}

	return S_OK;
}

void ScreenFlash::screenFlashing( float durationSec, float r, float g, float b )
{
	m_screenFlashDurationSec = durationSec;
	m_screenFlashCV.r = r;
	m_screenFlashCV.g = g;
	m_screenFlashCV.b = b;

	// 이 값에 0.0도를 주면 알아서 180도까지 올라가고 멈추게 된다.
	m_screenFlashAlphaAngle = 0.0f;
}

void ScreenFlash::reset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
						void* pUserContext )
{
	m_alphaShader->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
}