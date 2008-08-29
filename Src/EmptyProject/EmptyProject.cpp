//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//	
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Modified PoolG Team, a Division of PoolC
//
//
// Lifetime of the application
//
// Window and full screen Mode: Launch and Terminate
//		(1) WinMain
//		(2) OnCreateDevice
//		(3) OnResetDevice
//		(4) OnFrameMove / OnFrameRender loop
//		(5) OnLostDevice
//		(6) OnDestroyDevice
//
//--------------------------------------------------------------------------------------
#include "EmptyProjectPCH.h"
#include "EmptyProject.h"
#include "resource.h"

#include "BattleState.h"
#include "IntroState.h"
#include "EpCamera.h"
#include "MenuState.h"
#include "TopStateManager.h"
#include "ScriptManager.h"
#include "VideoMan.h"
#include "WorldStateManager.h"
#include "ShaderWrapper.h"
#include "TileManager.h"
#include "World.h"
#include "Dialog.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "EpLight.h"
#include "particlesystem.h"


SCREEN_VERTEX g_Vertex[4];

G								g_g;
WorldManager*					g_wm					= 0;
TopStateManager*				g_tsm					= 0;
WorldStateManager*				g_wsm					= 0;
ScriptManager*					g_scriptManager			= 0;		// Set to zero is 'CRUCIAL!'

SpriteManager*					g_spriteManager			= 0;
EpLight*						g_epLight				= 0;


LPD3DXEFFECT		            g_pEffect				= 0;
D3DXHANDLE						g_tech					= 0;
LPDIRECT3DVERTEXBUFFER9			g_lineElement			= 0;
HANDLE							g_scriptBindingFinishedEvent				= 0;		// Signal object to resolve multi-threaded problems on console thread and main app thread
HANDLE							g_consoleReleasedEvent = 0;		// Signal object to resolve multi-threaded problems on console thread and main app thread

LPD3DXMESH						g_testTeapot			= 0;

D3DCOLOR						g_fillColor;

LPD3DXFONT						g_pFont					= 0;		// ... Some globally used font?
LPD3DXFONT						g_unitNameFont			= 0;
LPD3DXFONT						g_dlgNameFont			= 0;
LPD3DXFONT						g_dlgContentFont		= 0;
// CreditState
LPD3DXFONT						g_d3dxFont				= 0;
LPD3DXFONT						g_d3dxFontBig			= 0;
// Battle State
LPD3DXFONT						m_lblHYnamL				= 0;
LPD3DXFONT						m_lblREB				= 0;
LPD3DXFONT						m_lblSkill				= 0;
LPD3DXFONT						m_lblSkillDescription	= 0;
LPD3DXFONT						m_lblStatSelect			= 0;


LOGMANAGER logMan;

Tcl_Interp*						g_consoleInterp			= 0;

std::wstring g_debugBuffer;
bool							g_bTileGrid				= false;





LPDIRECT3DTEXTURE9				g_pFullScreenRenderTarget		= 0;
LPDIRECT3DSURFACE9				g_pFullScreenRenderTargetSurf	= 0;

LPDIRECT3DTEXTURE9				g_sepiaRenderTarget				= 0;
LPDIRECT3DSURFACE9				g_sepiaRenderTargetSurf			= 0;

LPDIRECT3DTEXTURE9				g_radialBlurRenderTarget		= 0;
LPDIRECT3DSURFACE9				g_radialBlurRenderTargetSurf	= 0;

PostSepiaShader*				g_postSepiaShader				= 0;
PostRadialBlurShader*			g_postRadialBlurShader			= 0;

//MotionBlurShader*				g_motionBlurShader		= 0;
BombShader*						g_bombShader			= 0;


int								g_nActiveSystem = 0;
CParticleSystem					*g_pParticleSystems[6];
bool							g_bParticleVisible = false;

//////////////////////////////////////////////////////////////////////////

void ConfigureShaders( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
void ConfigureParticleSystem( LPDIRECT3DDEVICE9 pd3dDevice );
void ConfigureTileGridGeometry( LPDIRECT3DDEVICE9 pd3dDevice );
void ConfigureTestGeometry( LPDIRECT3DDEVICE9 pd3dDevice );			// TODO Not necessary on release build

void SetupFullscreenQuad( const D3DSURFACE_DESC* pBackBufferSurfaceDesc );


//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
									 bool bWindowed, void* pUserContext )
{
	// Typically want to skip back buffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	//pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_MULTITHREADED; // For multi-threaded console

	return true;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									void* pUserContext )
{
	OutputDebugString( _T( " - INFO: OnCreateDevice called.\n" ) );

	HRESULT hr = S_OK;

	UNREFERENCED_PARAMETER( hr );

	// (1) Set D3D device global variable which is shared through the whole application lifetime.
	//     GetG() related init
	assert( GetG().m_dev == 0 );
	assert( VideoMan::getSingleton().GetDev() == 0 );
	GetG().m_dev = pd3dDevice;
	VideoMan::getSingleton().SetDev( pd3dDevice );
	GetG().m_scrWidth = pBackBufferSurfaceDesc->Width;
	GetG().m_scrHeight = pBackBufferSurfaceDesc->Height;
	
	// Orthogonal and fixed view xforms for GUI or fixed element rendering
	D3DXVECTOR3 eye(0, 0, -50.0f), at(0, 0, 0), up(0, 1.0f, 0);
	D3DXMatrixOrthoLH(&GetG().g_orthoProjMat, (FLOAT)pBackBufferSurfaceDesc->Width, (FLOAT)pBackBufferSurfaceDesc->Height, 0.1f, 100.0f);
	D3DXMatrixLookAtLH(&GetG().g_fixedViewMat,	&eye, &at, &up);

	TCHAR scrSizeString[64];
	StringCchPrintf( scrSizeString, 64, _T( "- INFO: Window width: %d / height: %d\n" ), GetG().m_scrWidth, GetG().m_scrHeight );
	OutputDebugString( scrSizeString );

	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
	GetG().m_camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 1.0f, 1000.0f );
	
	// (x) Screen Flash (Alpha shader) init
	GetG().m_screenFlash.onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc );


	// (2) Configure shaders and particle system
	ConfigureShaders( pd3dDevice, pBackBufferSurfaceDesc );
	//ConfigureParticleSystem( pd3dDevice );
	
	// (3) Configure geometries (Vertex and index buffer manipulation)
	ConfigureTileGridGeometry( pd3dDevice );
	ConfigureTestGeometry( pd3dDevice );
	
	// (4) Light setup
	GetEpLight().setupLight();

	// (5) Globally used fonts
	
	V_RETURN( D3DXCreateFont( pd3dDevice, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulimche"), &g_pFont) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 26, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T( "Arial Black"), &g_unitNameFont ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulim"), &g_dlgContentFont) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("msgothic"), &g_dlgNameFont) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 26, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T( "Palatino Linotype"), &g_d3dxFont ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 32, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T( "Palatino Linotype"), &g_d3dxFontBig ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblHYnamL ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 18, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Rockwell Extra Bold"), &m_lblREB ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblSkill ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblSkillDescription ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblStatSelect ) );


	// (6) OnCreateDevice propagation
	GetTopStateManager().onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	GetWorldStateManager().onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	GetSpriteManager().onCreateDevice( pd3dDevice );
	GetWorldManager().onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
								   void* pUserContext )
{
	OutputDebugString( _T(" - INFO: OnResetDevice() called.\n") );

	GetG().m_dev = pd3dDevice;
	VideoMan::getSingleton().SetDev( pd3dDevice );
	GetG().m_scrWidth = pBackBufferSurfaceDesc->Width;
	GetG().m_scrHeight = pBackBufferSurfaceDesc->Height;

	HRESULT hr = S_OK;

	pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	//g_fillColor = D3DCOLOR_ARGB( 0, 45, 50, 170 );
	g_fillColor = D3DCOLOR_ARGB( 0, 0, 0, 0 );

	GetG().m_screenFlash.onResetDevice( pd3dDevice, pBackBufferSurfaceDesc );

	GetSpriteManager().onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	GetWorldManager().onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	GetTopStateManager().onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	GetWorldStateManager().onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );

	D3DFORMAT d3dFormat = D3DFMT_A8R8G8B8;

	// Create a A8R8G8B8 render target texture.  This will be used to render 
	// the full screen and then rendered to the backbuffer using a quad.
	V_RETURN( D3DXCreateTexture( pd3dDevice, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_pFullScreenRenderTarget ) );

	// Create sepia render target
	V_RETURN( D3DXCreateTexture( pd3dDevice, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height,
		1, D3DUSAGE_RENDERTARGET, d3dFormat, D3DPOOL_DEFAULT, &g_sepiaRenderTarget ) );

	// Create radial blur render target
	V_RETURN( D3DXCreateTexture( pd3dDevice, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height,
		1, D3DUSAGE_RENDERTARGET, d3dFormat, D3DPOOL_DEFAULT, &g_radialBlurRenderTarget ) );

	// Store pointers to surfaces so we can call SetRenderTarget() later
	V_RETURN( g_pFullScreenRenderTarget->GetSurfaceLevel( 0, &g_pFullScreenRenderTargetSurf ) );
	V_RETURN( g_sepiaRenderTarget->GetSurfaceLevel( 0, &g_sepiaRenderTargetSurf ) );
	V_RETURN( g_radialBlurRenderTarget->GetSurfaceLevel( 0, &g_radialBlurRenderTargetSurf ) );
	
	g_postSepiaShader->setFullscreenTexture( g_sepiaRenderTarget );
	g_postRadialBlurShader->setFullscreenTexture( g_radialBlurRenderTarget );

	SetupFullscreenQuad( pBackBufferSurfaceDesc );

	
	g_pFont->OnResetDevice();
	g_unitNameFont->OnResetDevice();
	g_dlgNameFont->OnResetDevice();
	g_dlgContentFont->OnResetDevice();

	m_lblHYnamL->OnResetDevice();
	m_lblREB->OnResetDevice();
	m_lblSkill->OnResetDevice();
	m_lblSkillDescription->OnResetDevice();
	m_lblStatSelect->OnResetDevice();


	g_postRadialBlurShader->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc );
	g_postSepiaShader->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc );
	//g_motionBlurShader->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc );
	g_bombShader->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc );

	return hr;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{

	HRESULT hr;

	UNREFERENCED_PARAMETER( hr );

	GetWorldManager().changeToNextWorldIfExist();

	//////////////////////////////////////////////////////////////////////////

	TopStateManager::getSingleton().transit();

	if (GetTopStateManager().getCurState())
		GetTopStateManager().getCurState()->frameMove(fTime, fElapsedTime);

	GetG().m_camera.frameMove( fElapsedTime );
	GetEpLight().frameMove( fElapsedTime );


	WCHAR msg[128];
	
	StringCchPrintf( msg, 128, L"Camera Pos : (%.2f, %.2f, %.2f)\n",
		GetG().m_camera.GetEyePt()->x,
		GetG().m_camera.GetEyePt()->y,
		GetG().m_camera.GetEyePt()->z );
	g_debugBuffer.append( msg );

	StringCchPrintf( msg, 128, L"Camera LookAt : (%.2f, %.2f, %.2f)\n",
		GetG().m_camera.GetLookAtPt()->x,
		GetG().m_camera.GetLookAtPt()->y,
		GetG().m_camera.GetLookAtPt()->z );
	g_debugBuffer.append( msg );
	
	GetG().m_screenFlash.frameMove( fTime, fElapsedTime );
}


HRESULT drawBurningTeapot( double fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	UINT iPass, cPasses;
	V( g_bombShader->setMainTechnique() );
	V( g_bombShader->setWorldViewProj( fTime, fElapsedTime, &mWorld, GetG().m_camera.GetViewMatrix(), GetG().m_camera.GetProjMatrix() ) );

	V( g_bombShader->begin( &cPasses, 0 ) );
	for( iPass = 0; iPass < cPasses; iPass++ )
	{
		V( g_bombShader->beginPass( iPass ) );
		// Draw mesh here...

		//D3DPERF_BeginEvent( 0, L"Shader applied teapot" );
		g_testTeapot->DrawSubset( 0 );
		//D3DPERF_EndEvent();

		V( g_bombShader->endPass() );
	}
	V( g_bombShader->end() );
	return hr;
}


void renderDebugText()
{
	EpCamera& g_camera = GetG().m_camera;

	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = GetG().m_scrWidth;
	rc.bottom = GetG().m_scrHeight;

	D3DXCOLOR textColor = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
	g_pFont->DrawTextW( 0, g_debugBuffer.c_str(), -1, &rc, DT_NOCLIP | DT_RIGHT, textColor );
}

void renderFixedElements( double fTime, float fElapsedTime )
{
	DXUTGetD3D9Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	DXUTGetD3D9Device()->SetTransform(D3DTS_VIEW, &GetG().g_fixedViewMat);
	DXUTGetD3D9Device()->SetTransform(D3DTS_PROJECTION, &GetG().g_orthoProjMat);

#ifdef DEBUG
	renderDebugText();
#endif

	g_debugBuffer.clear();

}

HRESULT renderTileGrid()
{
	HRESULT hr = S_OK;

	GetG().m_dev->SetVertexShader( 0 );
	GetG().m_dev->SetRenderState( D3DRS_LIGHTING, FALSE );
	GetG().m_dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	GetG().m_dev->SetTexture( 0, 0 );

	V( GetG().m_dev->SetTransform( D3DTS_VIEW, GetG().m_camera.GetViewMatrix() ) );
	V( GetG().m_dev->SetTransform( D3DTS_PROJECTION, GetG().m_camera.GetProjMatrix() ) );
	V( GetG().m_dev->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE ) );
	V( GetG().m_dev->SetStreamSource( 0, g_lineElement, 0, 16 ) );
	int i;
	D3DXMATRIX mWorld, mTrans, mRot, mScaling;
	D3DXMatrixScaling( &mScaling, (float)(s_xSize) * s_tileSize, 1.0f, 1.0f );
	for ( i = -s_ySize / 2; i < s_ySize / 2; ++i )
	{
		D3DXMatrixTranslation( &mTrans,
			0,
			(float)i * s_tileSize,
			0 );
		mWorld = mScaling * mTrans;
		V( GetG().m_dev->SetTransform( D3DTS_WORLD, &mWorld ) );
		V( GetG().m_dev->DrawPrimitive( D3DPT_LINELIST, 0, 1 ) );
	}

	D3DXMatrixScaling( &mScaling, (float)(s_ySize) * s_tileSize, 1.0f, 1.0f );
	D3DXMatrixRotationZ( &mRot, D3DXToRadian( 90 ) );
	for ( i = -s_xSize / 2; i < s_xSize / 2; ++i )
	{
		D3DXMatrixTranslation( &mTrans,
			(float)i * s_tileSize,
			0,
			0 );
		mWorld = mScaling * mRot * mTrans;
		V( GetG().m_dev->SetTransform( D3DTS_WORLD, &mWorld ) );
		D3DPERF_BeginEvent( 0, L"Boundary Line Drawing" );
		V( GetG().m_dev->DrawPrimitive( D3DPT_LINELIST, 0, 1 ) );
		D3DPERF_EndEvent();
	}

	return hr;
}
void SetupFullscreenQuad( const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	D3DSURFACE_DESC desc;

	g_pFullScreenRenderTargetSurf->GetDesc( &desc );

	// Ensure that we're directly mapping texels to pixels by offset by 0.5
	// For more info see the doc page titled "Directly Mapping Texels to Pixels"
	FLOAT fWidth5 = ( FLOAT )pBackBufferSurfaceDesc->Width - 0.5f;
	FLOAT fHeight5 = ( FLOAT )pBackBufferSurfaceDesc->Height - 0.5f;

	FLOAT fTexWidth1 = ( FLOAT )pBackBufferSurfaceDesc->Width / ( FLOAT )desc.Width;
	FLOAT fTexHeight1 = ( FLOAT )pBackBufferSurfaceDesc->Height / ( FLOAT )desc.Height;

	// Fill in the vertex values
	g_Vertex[0].pos = D3DXVECTOR4( fWidth5, -0.5f, 0.0f, 1.0f );
	g_Vertex[0].clr = D3DXCOLOR( 0.5f, 0.5f, 0.5f, 0.66666f );
	g_Vertex[0].tex1 = D3DXVECTOR2( fTexWidth1, 0.0f );

	g_Vertex[1].pos = D3DXVECTOR4( fWidth5, fHeight5, 0.0f, 1.0f );
	g_Vertex[1].clr = D3DXCOLOR( 0.5f, 0.5f, 0.5f, 0.66666f );
	g_Vertex[1].tex1 = D3DXVECTOR2( fTexWidth1, fTexHeight1 );

	g_Vertex[2].pos = D3DXVECTOR4( -0.5f, -0.5f, 0.0f, 1.0f );
	g_Vertex[2].clr = D3DXCOLOR( 0.5f, 0.5f, 0.5f, 0.66666f );
	g_Vertex[2].tex1 = D3DXVECTOR2( 0.0f, 0.0f );

	g_Vertex[3].pos = D3DXVECTOR4( -0.5f, fHeight5, 0.0f, 1.0f );
	g_Vertex[3].clr = D3DXCOLOR( 0.5f, 0.5f, 0.5f, 0.66666f );
	g_Vertex[3].tex1 = D3DXVECTOR2( 0.0f, fTexHeight1 );
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{

	HRESULT hr;

	LPDIRECT3DSURFACE9 originalRT = 0;
	V( pd3dDevice->GetRenderTarget( 0, &originalRT ) );


	// Clear the render target and the zbuffer 
	V( pd3dDevice->SetRenderTarget( 0, g_radialBlurRenderTargetSurf	 ) );
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, g_fillColor, 1.0f, 0 ) );
	V( pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE ) );
	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		//drawBurningTeapot( fTime, fElapsedTime );

		if ( g_bTileGrid )
			renderTileGrid();

		State* curState = GetTopStateManager().getCurState();

		curState->frameRender(pd3dDevice, fTime, fElapsedTime);

		renderFixedElements( fTime, fElapsedTime );

		g_spriteManager->frameRender();
		


		//////////////////////////////////////////////////////////////////////////
		V( pd3dDevice->EndScene() );
	}

	//GetG().m_screenFlash.frameRender();


	V( pd3dDevice->SetRenderTarget( 0, g_sepiaRenderTargetSurf ) );
	V( pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE ) );

	// Clear the render target
	V( pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L ) );
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		UINT iPass, cPasses;
		V( g_postRadialBlurShader->setMainTechnique() );
		V( g_postRadialBlurShader->begin( &cPasses, 0 ) );
		for( iPass = 0; iPass < cPasses; iPass++ )
		{
			V( g_postRadialBlurShader->beginPass( iPass ) );
			V( pd3dDevice->SetFVF( SCREEN_VERTEX::FVF ) );
			V( pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_Vertex, sizeof( SCREEN_VERTEX ) ) );
			V( g_postRadialBlurShader->endPass() );
		}
		V( g_postRadialBlurShader->end() );
		//////////////////////////////////////////////////////////////////////////
		V( pd3dDevice->EndScene() );
	}


	V( pd3dDevice->SetRenderTarget( 0, originalRT ) );
	

	// Clear the render target
	V( pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L ) );
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		UINT iPass, cPasses;
		V( g_postSepiaShader->setMainTechnique() );
		V( g_postSepiaShader->begin( &cPasses, 0 ) );
		for( iPass = 0; iPass < cPasses; iPass++ )
		{
			V( g_postSepiaShader->beginPass( iPass ) );
			V( pd3dDevice->SetFVF( SCREEN_VERTEX::FVF ) );
			V( pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_Vertex, sizeof( SCREEN_VERTEX ) ) );
			V( g_postSepiaShader->endPass() );
		}
		V( g_postSepiaShader->end() );
		//////////////////////////////////////////////////////////////////////////
		V( pd3dDevice->EndScene() );
	}

	SAFE_RELEASE( originalRT );


	if ( g_bParticleVisible )
	{

		//////////////////////////////////////////////////////////////////////////
		//
		// Transform for particle systems
		//

		pd3dDevice->SetTransform( D3DTS_VIEW, GetG().m_camera.GetViewMatrix() );
		pd3dDevice->SetTransform( D3DTS_PROJECTION, GetG().m_camera.GetProjMatrix() );
		D3DXMATRIX world;	
		D3DXMATRIX scale;
		D3DXMATRIX rotate;
		D3DXMATRIX trans;
		D3DXMatrixScaling( &scale, 0.5f, 0.5f, 0.5f );
		D3DXMatrixRotationX( &rotate, D3DXToRadian( -90.0f ) );	
		D3DXMatrixTranslation( &trans, -30.0f, -4.0f, 0.0f );
		world = rotate * scale * trans;
		pd3dDevice->SetTransform( D3DTS_WORLD, &world );

		pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
		
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//
		// Render particle systems
		//
		// The particle system will need to know how much time has passed since 
		// the last time it was updated, so we'll need to keep track of how much   
		// time has elapsed since the last frame update...
		//

		g_pParticleSystems[g_nActiveSystem]->Update( (float)fElapsedTime );

		pd3dDevice->BeginScene();

		//
		// Prepare to render particle system
		//

		//
		// Setting D3DRS_ZWRITEENABLE to FALSE makes the Z-Buffer read-only, which 
		// helps remove graphical artifacts generated from  rendering a list of 
		// particles that haven't been sorted by distance to the eye.
		//
		// Setting D3DRS_ALPHABLENDENABLE to TRUE allows particles, which overlap, 
		// to alpha blend with each other correctly.
		//

		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

		//
		// Render particle system
		//

		pd3dDevice->SetTexture( 0, g_pParticleSystems[g_nActiveSystem]->GetTextureObject() );
		g_pParticleSystems[g_nActiveSystem]->Render( pd3dDevice );

		//
		// Reset render states...
		//

		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

		pd3dDevice->EndScene();
		pd3dDevice->Present( NULL, NULL, NULL, NULL );

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
	}
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
						 bool* pbNoFurtherProcessing, void* pUserContext )
{
	EpCamera& g_camera = GetG().m_camera;

	g_camera.handleMessages(hWnd, uMsg, wParam, lParam);
	GetEpLight().handleMessages(hWnd, uMsg, wParam, lParam);
	GetG().m_screenFlash.handleMessage( hWnd, uMsg, wParam, lParam );

	if (g_tsm && GetTopStateManager().getCurState())
		GetTopStateManager().getCurState()->handleMessages(hWnd, uMsg, wParam, lParam);

	if ( uMsg == WM_KEYDOWN )
	{
		if ( wParam == 'P' )
		{
			g_nActiveSystem++;
			g_nActiveSystem %= 6;	
		}
		else if ( wParam == 'L' )
		{
			g_bParticleVisible = g_bParticleVisible?false:true;
		}
	}

	return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	OutputDebugString( _T( " - INFO: OnLostDevice() called.\n" ) );

	if ( g_bombShader )
		g_bombShader->onLostDevice();
	if ( g_pFont )
		g_pFont->OnLostDevice();
	if ( g_unitNameFont )
		g_unitNameFont->OnLostDevice();
	if ( g_dlgNameFont )
		g_dlgNameFont->OnLostDevice();
	if ( g_dlgContentFont )
		g_dlgContentFont->OnLostDevice();

	if ( SpriteManager::getSingletonPtr() )
		GetSpriteManager().onLostDevice();

	if ( WorldManager::getSingletonPtr() )
		GetWorldManager().onLostDevice();

	if ( TopStateManager::getSingletonPtr() )
		GetTopStateManager().onLostDevice();

	if ( WorldStateManager::getSingletonPtr() )
		GetWorldStateManager().onLostDevice();

	SAFE_RELEASE( g_pFullScreenRenderTarget );
	SAFE_RELEASE( g_pFullScreenRenderTargetSurf );
	SAFE_RELEASE( g_sepiaRenderTarget );
	SAFE_RELEASE( g_sepiaRenderTargetSurf );
	SAFE_RELEASE( g_radialBlurRenderTarget );
	SAFE_RELEASE( g_radialBlurRenderTargetSurf );

	g_postRadialBlurShader->onLostDevice();
	g_postSepiaShader->onLostDevice();
	//g_motionBlurShader->onLostDevice();
	g_bombShader->onLostDevice();

	GetG().m_screenFlash.onLostDevice();

	// GetG() related
	GetG().m_dev = 0;
	GetG().m_videoMan.SetDev(0);
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	OutputDebugString( _T( " - INFO: OnDestroyDevice() called.\n" ) );

	// Globally used fonts
	SAFE_RELEASE( g_pFont );
	SAFE_RELEASE( g_unitNameFont );
	SAFE_RELEASE( g_dlgNameFont );
	SAFE_RELEASE( g_dlgContentFont );
	SAFE_RELEASE( g_d3dxFont );
	SAFE_RELEASE( g_d3dxFontBig );

	// Test geometries
	SAFE_RELEASE( g_testTeapot );

	// Tile grid geometry
	SAFE_RELEASE( g_lineElement );

	// Shaders
	EP_SAFE_RELEASE( g_bombShader );
	//EP_SAFE_RELEASE( g_motionBlurShader );
	EP_SAFE_RELEASE( g_postSepiaShader );
	EP_SAFE_RELEASE( g_postRadialBlurShader );


	// GetG() related
	GetG().m_dev = 0;
	GetG().m_videoMan.SetDev(0);
	GetG().m_screenFlash.onDestroyDevice();


	for( int i = 0; i < 6; ++i )
	{
		if( g_pParticleSystems[i] != NULL )
		{
			delete g_pParticleSystems[i];
			g_pParticleSystems[i] = NULL;
		}
	}


	SAFE_RELEASE( m_lblHYnamL );
	SAFE_RELEASE( m_lblREB );
	SAFE_RELEASE( m_lblSkill );
	SAFE_RELEASE( m_lblSkillDescription );
	SAFE_RELEASE( m_lblStatSelect );


	GetSpriteManager().onDestroyDevice();

	GetG().m_screenFlash.onDestroyDevice();
}


//--------------------------------------------------------------------------------------
// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( bKeyDown )
	{
		switch( nChar )
		{
		case VK_F1:
			if (g_fillColor == D3DCOLOR_ARGB( 0, 45, 50, 170 ))
			{
				g_fillColor = D3DCOLOR_ARGB(0, 200, 100, 25);
			}
			else
			{
				g_fillColor = D3DCOLOR_ARGB( 0, 45, 50, 170 );
			}
			break;
		}
	}
	else
	{
		// Key up
		switch ( nChar )
		{
		case VK_F3:
			g_bTileGrid = !g_bTileGrid;
			break;
		case VK_F4:
			GetG().m_camera.setShake( !GetG().m_camera.getShake() );
			break;
		}
	}
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


static bool isCurrentWorkingDir = false;
void SetCurrentWorkingDirectory()
{
	if ( isCurrentWorkingDir == false )
	{
		TCHAR buf[MAX_PATH];
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		GetModuleFileName(NULL, buf, MAX_PATH);
		_tsplitpath_s(buf, drive, _MAX_DRIVE, dir, _MAX_DIR, 0, 0, 0, 0);
		StringCchPrintf(buf, MAX_PATH, _T("%s%s"), drive, dir);
		SetCurrentDirectory(buf);

		isCurrentWorkingDir = true;
	}
}


int EpOutputDebugString( const char* msg )
{
	OutputDebugStringA( msg );
	return 0;

} SCRIPT_CALLABLE_I_PC( EpOutputDebugString )



//#define EP_CONSOLE

static int g_closeConsole = 2008;

void EpConsoleThreadMain( void* cd )
{
#if defined(DEBUG) && defined(EP_CONSOLE)
	g_consoleInterp = Tcl_CreateInterp();
	if (Tcl_Init(g_consoleInterp) == TCL_ERROR)
		throw std::runtime_error( "Tcl_Init() error" );
#endif

	GetScriptManager().initScriptBindings();

	CREATE_OBJ_COMMAND( EpOutputDebugString );


	SetEvent( g_scriptBindingFinishedEvent );

#if defined(DEBUG) && defined(EP_CONSOLE)
	if ( Tcl_LinkVar( g_consoleInterp, "closeConsole", (char*)&g_closeConsole, TCL_LINK_INT ) != TCL_OK )
	{
		DebugBreak();
	}

	if ( Tcl_EvalFile( g_consoleInterp, "Script/EpThreadTest.tcl" ) != TCL_OK )
		ScriptManager::throwScriptErrorWithMessage( g_consoleInterp );


	// Could not reach here...
	Tcl_DeleteInterp( g_consoleInterp );
	g_consoleInterp = 0;

	SetEvent( g_consoleReleasedEvent );

#endif
	return;
}


void CreateScriptManagerIfNotExist()
{
	SetCurrentWorkingDirectory();
	if ( !g_scriptManager )
	{
		g_scriptManager = new ScriptManager();


		GetScriptManager().init();
	}
}



//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
#ifdef DEBUG
int main()
{
	system( "title EmptyProject Console" );
#else
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
#endif
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	// TODO: Perform any application-level initialization here
	// There is no dependency on D3D device in application-level init step.(and should be)
	SetCurrentWorkingDirectory();
	g_debugBuffer.resize( 2048 );
	
	// (1) Script Manager Initialization
	CreateScriptManagerIfNotExist();
	GetScriptManager().executeFile( "Script/EpInitScript.tcl" );
	GetScriptManager().executeFile( "Script/EpDialog1.tcl" );
	GetScriptManager().executeFile( "Script/EpSkill.tcl" );
	GetScriptManager().execute( "EpInitApp" );


	// (9) Sprite manager
	assert( g_spriteManager == 0 );
	g_spriteManager = new SpriteManager();

	// (2) Top State Manager (tsm)
	// TODO: State preservation is needed!!! NO CREATION ON EVERY OnCreateDevice()!
	assert( g_tsm == 0 );
	g_tsm = new TopStateManager();
	g_tsm->init();

	// (3) World State Manager (wsm)
	// TODO: State preservation is needed!!! NO CREATION ON EVERY OnCreateDevice()!
	assert( g_wsm == 0 );
	g_wsm = new WorldStateManager();
	g_wsm->init();
	
	// (4) World Manager
	assert( g_wm == 0 );
	g_wm = new WorldManager();

	// (5) Read all world script files
	assert( GetWorldManager().getWorldCount() == 0 );
	ConstCharList worldList;
	GetScriptManager().readCharPtrList( "EpWorldList", worldList );
	char scriptCommand[512];
	ConstCharList::iterator it = worldList.begin();
	for ( ; it != worldList.end(); ++it )
	{
		const char* worldName = *it;

		StringCchPrintfA( scriptCommand, 512, "Script/%s.tcl", worldName );
		GetScriptManager().executeFile( scriptCommand );
		StringCchPrintfA( scriptCommand, 512, "%s::modelFilePath", worldName );
		const char* modelFilePath = GetScriptManager().readString( scriptCommand );
		WCHAR mfp[128];
		size_t numOfConvert;
		mbstowcs_s( &numOfConvert, mfp, 128, modelFilePath, 512 );
		World* world = World::createWorld( worldName, mfp, false );
		GetWorldManager().addWorld( world );
	}

	// (6) Camera Setup
	GetG().m_camera.SetAttachCameraToModel( true );
	GetG().m_camera.SetEnablePositionMovement( true );

	// (7) EpLight
	assert( g_epLight == 0 );
	g_epLight = new EpLight();

	// (8) Ep Console Thread
	g_scriptBindingFinishedEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	ResetEvent( g_scriptBindingFinishedEvent );
	uintptr_t t = _beginthread( EpConsoleThreadMain, 0, 0 );
	WaitForSingleObject( g_scriptBindingFinishedEvent, INFINITE );



	// (10) Script side callback invocation (Do at the end of this function.)
	GetScriptManager().execute( "EpOnCreateDevice" );

	// (11) Set the first world
	// TODO: This is 'DO ONCE THROUGH THE WHOLE LIFETIME job.. not on every OnCreateDevice()!
	const char* startWorldName = GetScriptManager().readString( "EpStartWorldName" );
	GetWorldManager().setNextWorld( startWorldName );
	GetWorldManager().changeToNextWorldIfExist();

	// (12) Determine Windowed or full screen mode
	const char* windowMode = GetScriptManager().readString( "EpWindowMode" );
	bool bWindowMode = (*windowMode)=='1'?true:false;


	//////////////////////////////////////////////////////////////////////////

	// Set the DXUT callback functions
	DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
	DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
	DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
	DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
	DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
	DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackKeyboard( KeyboardProc );

	// Initialize DXUT and create the desired Win32 window and Direct3D device for the application
	DXUTInit( true, true ); // Parse the command line and show msgboxes
	DXUTSetHotkeyHandling( true, false, true );  // handle the default hotkeys
	DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
	DXUTCreateWindow( L"EmptyProject" );
	if ( bWindowMode )
	{
		// Window size is determined by script
		DXUTCreateDevice( true, GetG().m_scrWidth, GetG().m_scrHeight );
	}
	else
	{
		// Window size is determined by monitor resolution.
		DXUTCreateDevice( false );
	}

	DXUTMainLoop();

	DXUTShutdown();

	//////////////////////////////////////////////////////////////////////////

	// TODO: Perform any application-level cleanup here
	
#if defined(DEBUG) && defined(EP_CONSOLE)
	g_consoleReleasedEvent = CreateEvent( NULL , TRUE , FALSE , NULL );  
	ResetEvent( g_consoleReleasedEvent ); 
	g_closeConsole = 1;
	WaitForSingleObject( g_consoleReleasedEvent, INFINITE );
	
#endif

	// DO NOT USE EP_SAFE_RELEASE or SAFE_DELETE on these objects.
	// It should be allocated once time through the application lifetime,
	// and therefore should be deallocated only once.

	g_wm->release();
	g_scriptManager->release(); 
	g_tsm->release();
	g_wsm->release();

	delete g_wm;
	delete g_scriptManager;
	delete g_tsm;
	delete g_wsm;
	delete g_spriteManager;

	delete g_epLight;
	
	Tcl_Finalize();

	return DXUTGetExitCode();
}


void ConfigureParticleSystem( LPDIRECT3DDEVICE9 pd3dDevice )
{
	UINT i;
	for ( i = 0; i < 6; ++i )
	{
		assert( g_pParticleSystems[i] == 0 );
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// Particle systems initialize
	//
	// Exploding burst
	//
	g_pParticleSystems[0] = new CParticleSystem();

	g_pParticleSystems[0]->SetTexture( "Images/particle.bmp" );
	g_pParticleSystems[0]->SetMaxParticles( 100 );
	g_pParticleSystems[0]->SetNumToRelease( 100 );
	g_pParticleSystems[0]->SetReleaseInterval( 0.05f );
	g_pParticleSystems[0]->SetLifeCycle( 0.5f );
	g_pParticleSystems[0]->SetSize( 1.0f );
	g_pParticleSystems[0]->SetColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ));
	g_pParticleSystems[0]->SetPosition( D3DXVECTOR3( 0.0f, 5.0f, 0.0f) );
	g_pParticleSystems[0]->SetVelocity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	g_pParticleSystems[0]->SetGravity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	g_pParticleSystems[0]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	g_pParticleSystems[0]->SetVelocityVar( 10.0f );	

	g_pParticleSystems[0]->Init( pd3dDevice );

	//
	// Wind blown fountain
	//

	
	g_pParticleSystems[1] = new CParticleSystem();

	g_pParticleSystems[1]->SetTexture( "Images/particle.bmp" );
	g_pParticleSystems[1]->SetMaxParticles( 500 );
	g_pParticleSystems[1]->SetNumToRelease( 5 );
	g_pParticleSystems[1]->SetReleaseInterval( 0.05f );
	g_pParticleSystems[1]->SetLifeCycle( 4.0f );
	g_pParticleSystems[1]->SetSize( 0.5f );
	g_pParticleSystems[1]->SetColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
	g_pParticleSystems[1]->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[1]->SetVelocity( D3DXVECTOR3( 0.0f, 5.0f, 0.0f ) );
	g_pParticleSystems[1]->SetGravity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[1]->SetWind( D3DXVECTOR3( 2.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[1]->SetVelocityVar( 1.5f );

	g_pParticleSystems[1]->Init( pd3dDevice );

	//
	// Omni-directiional emission expanding into space with no air resistence
	//

	g_pParticleSystems[2] = new CParticleSystem();

	g_pParticleSystems[2]->SetTexture( "Images/particle.bmp" );
	g_pParticleSystems[2]->SetMaxParticles( 2048 );
	g_pParticleSystems[2]->SetNumToRelease( 10 );
	g_pParticleSystems[2]->SetReleaseInterval( 0.05f );
	g_pParticleSystems[2]->SetLifeCycle( 5.0f );
	g_pParticleSystems[2]->SetSize( 0.5f );
	g_pParticleSystems[2]->SetColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
	g_pParticleSystems[2]->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	g_pParticleSystems[2]->SetVelocity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	g_pParticleSystems[2]->SetGravity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );

	g_pParticleSystems[2]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	g_pParticleSystems[2]->SetAirResistence( false );

	g_pParticleSystems[2]->SetVelocityVar(2.0f);

	g_pParticleSystems[2]->Init( pd3dDevice );

	//
	// Fountain particles behave like paint spots when striking a plane as 
	// the wind blowing them towards us
	//

	g_pParticleSystems[3] = new CParticleSystem();

	g_pParticleSystems[3]->SetTexture( "Images/particle.bmp" );
	g_pParticleSystems[3]->SetMaxParticles( 100 );
	g_pParticleSystems[3]->SetNumToRelease( 10 );
	g_pParticleSystems[3]->SetReleaseInterval( 0.05f );
	g_pParticleSystems[3]->SetLifeCycle( 3.0f );
	g_pParticleSystems[3]->SetSize( 0.5f );
	g_pParticleSystems[3]->SetColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
	g_pParticleSystems[3]->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[3]->SetVelocity( D3DXVECTOR3( 0.0f, 5.0f, 0.0f ) );
	g_pParticleSystems[3]->SetGravity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[3]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, -20.0f ) );
	g_pParticleSystems[3]->SetVelocityVar( 2.5f );

	g_pParticleSystems[3]->SetCollisionPlane( D3DXVECTOR3( 0.0f, 0.0f,1.0f ), 
		D3DXVECTOR3( 0.0f, 0.0f, -5.0f ),
		1.0f, CR_STICK );

	g_pParticleSystems[3]->Init( pd3dDevice );

	//
	// Fountain using a single collision plane acting as a floor
	//

	g_pParticleSystems[4] = new CParticleSystem();

	g_pParticleSystems[4]->SetTexture( "Images/particle.bmp" );
	g_pParticleSystems[4]->SetMaxParticles( 200 );
	g_pParticleSystems[4]->SetNumToRelease( 10 );
	g_pParticleSystems[4]->SetReleaseInterval( 0.05f );
	g_pParticleSystems[4]->SetLifeCycle( 5.0f );
	g_pParticleSystems[4]->SetSize( 1.5f );
	g_pParticleSystems[4]->SetColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
	g_pParticleSystems[4]->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[4]->SetVelocity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[4]->SetGravity( D3DXVECTOR3( 0.0f, -9.8f, 0.0f ) );
	g_pParticleSystems[4]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[4]->SetVelocityVar( 20.0f );

	g_pParticleSystems[4]->SetCollisionPlane( D3DXVECTOR3( 0.0f, 1.0f, 0.0f ), 
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	g_pParticleSystems[4]->Init( pd3dDevice );

	//
	// Fountain boxed-in by 6 collision planes
	//

	g_pParticleSystems[5] = new CParticleSystem();

	g_pParticleSystems[5]->SetTexture( "Images/particle.bmp" );
	g_pParticleSystems[5]->SetMaxParticles( 100 );
	g_pParticleSystems[5]->SetNumToRelease( 5 );
	g_pParticleSystems[5]->SetReleaseInterval( 0.05f );
	g_pParticleSystems[5]->SetLifeCycle( 5.0f );
	g_pParticleSystems[5]->SetSize( 1.0f );
	g_pParticleSystems[5]->SetColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
	g_pParticleSystems[5]->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[5]->SetVelocity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[5]->SetGravity( D3DXVECTOR3( 0.0f, -9.8f, 0.0f ) );
	g_pParticleSystems[5]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	g_pParticleSystems[5]->SetVelocityVar( 20.0f );

	// Create a series of planes to collide with
	g_pParticleSystems[5]->SetCollisionPlane( D3DXVECTOR3( 0.0f, 1.0f, 0.0f ), 
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) ); // Floor

	g_pParticleSystems[5]->SetCollisionPlane( D3DXVECTOR3( 1.0f, 0.0f, 0.0f ), 
		D3DXVECTOR3(-3.0f, 0.0f, 0.0f ) ); // Left Wall

	g_pParticleSystems[5]->SetCollisionPlane( D3DXVECTOR3(-1.0f, 0.0f, 0.0f ), 
		D3DXVECTOR3( 3.0f, 0.0f, 0.0f ) ); // Right Wall

	g_pParticleSystems[5]->SetCollisionPlane( D3DXVECTOR3( 0.0f, 0.0f, 1.0f ), 
		D3DXVECTOR3( 0.0f, 0.0f,-3.0f ) ); // Front Wall

	g_pParticleSystems[5]->SetCollisionPlane( D3DXVECTOR3( 0.0f, 0.0f,-1.0f ), 
		D3DXVECTOR3( 0.0f, 0.0f, 3.0f ) ); // Back Wall

	g_pParticleSystems[5]->SetCollisionPlane( D3DXVECTOR3( 0.0f,-1.0f, 0.0f ), 
		D3DXVECTOR3( 0.0f, 5.0f, 0.0f ) ); // Ceiling

	g_pParticleSystems[5]->Init( pd3dDevice );

}

void ConfigureShaders( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	// Shader
	assert( g_bombShader == 0 );
	g_bombShader = new BombShader();
	g_bombShader->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc );
	g_bombShader->initMainTechnique();

	/*assert( g_motionBlurShader == 0 );
	g_motionBlurShader = new MotionBlurShader();
	g_motionBlurShader->initShader( pd3dDevice, L"" );*/

	assert( g_postSepiaShader == 0 );
	g_postSepiaShader = new PostSepiaShader();
	g_postSepiaShader->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc );
	g_postSepiaShader->initMainTechnique();

	assert( g_postRadialBlurShader == 0 );
	g_postRadialBlurShader = new PostRadialBlurShader();
	g_postRadialBlurShader->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc );
	g_postRadialBlurShader->initMainTechnique();

}

void ConfigureTileGridGeometry( LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr = S_OK;

	assert( g_lineElement == 0 );

	V( pd3dDevice->CreateVertexBuffer( sizeof( MY_COLOR_VERTEX ) * 2, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &g_lineElement, 0 ) );

	const D3DCOLOR gridColor = D3DCOLOR_ARGB( 255, 32, 128, 128 );
	MY_COLOR_VERTEX data[] = {
		{ -0.5f, 0.0f, -0.2f, gridColor },
		{ +0.5f, 0.0f, -0.2f, gridColor }
	};

	void* pVB = 0;
	V( g_lineElement->Lock( 0, 0, &pVB, 0 ) );
	memcpy( pVB, data, sizeof( data ) );
	g_lineElement->Unlock();
}

void ConfigureTestGeometry( LPDIRECT3DDEVICE9 pd3dDevice )
{
	assert( g_testTeapot == 0 );
	D3DXCreateTeapot( pd3dDevice, &g_testTeapot, 0 );
}