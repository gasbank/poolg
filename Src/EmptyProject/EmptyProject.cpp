//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//	
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Modified PoolG Team, a Division of PoolC
//
//--------------------------------------------------------------------------------------
#include "EmptyProjectPCH.h"
#include <time.h>
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

G								g_g;
TopStateManager*				g_tsm					= 0;
WorldStateManager*				g_wsm					= 0;
ScriptManager*					g_scriptManager			= 0;		// Set to zero is 'CRUCIAL!'
BombShader*						g_bombShader			= 0;
AlphaShader*					g_alphaShader			= 0;

LPD3DXFONT						g_pFont					= 0;
LPD3DXEFFECT		            g_pEffect				= 0;
D3DXHANDLE						g_tech					= 0;
LPD3DXLINE						g_line					= 0;

LPD3DXMESH						g_testTeapot			= 0;
LPD3DXMESH						g_testPolygon			= 0;
LPD3DXMESH						g_testPolygonCloned		= 0;

D3DCOLOR						g_fillColor;

LOGMANAGER logMan;


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
	assert( DXUT_D3D9_DEVICE == pDeviceSettings->ver );

	HRESULT hr;
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	D3DCAPS9 caps;
	V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal,
		pDeviceSettings->d3d9.DeviceType,
		&caps ) );

	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
	// then switch to SWVP.
	if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION( 2, 0 ) )
	{
		pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// Debugging vertex shaders requires either REF or software vertex processing 
	// and debugging pixel shaders requires REF.
#ifdef DEBUG_VS
	if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
	{
		pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
		pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
#endif
#ifdef DEBUG_PS
	pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif

	// For the first device created if its a REF device, optionally display a warning dialog box
	static bool s_bFirstTime = true;
	if( s_bFirstTime )
	{
		s_bFirstTime = false;
		if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF )
		{
			MessageBox( 0, L"REF mode will run very slowly", L"Caution", MB_ICONEXCLAMATION | MB_OK );
		}
	}

	pDeviceSettings->d3d9.pp.EnableAutoDepthStencil= TRUE;
	pDeviceSettings->d3d9.pp.AutoDepthStencilFormat = D3DFMT_D16;

	return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
	HRESULT hr;
	GetG().m_dev = pd3dDevice;
	//////////////////////////////////////////////////////////////////////////

	// State Manager Initialization
	g_tsm = new TopStateManager();
	g_wsm = new WorldStateManager();

	// Init Script
	GetScriptManager().execute( "EpInitGame" );

	// Shader
	g_bombShader = new BombShader();
	g_bombShader->initEffect( pd3dDevice, L"Shaders/HLSL/vbomb.fx" );
	g_bombShader->initMainTechnique();

	g_alphaShader = new AlphaShader();
	g_alphaShader->initShader( pd3dDevice, L"Shaders/Alpha.vsh" );
	g_alphaShader->compileShader( "Alpha", "vs_2_0" );

	D3DXCreateTeapot( pd3dDevice, &g_testTeapot, 0 );
	D3DXCreatePolygon( pd3dDevice, 0.1f, 32, &g_testPolygon, 0 );

	g_testPolygon->CloneMesh( 0, g_alphaShader->getDecl(), pd3dDevice, &g_testPolygonCloned );

	D3DXCreateLine( pd3dDevice, &g_line );
	g_line->SetAntialias( TRUE );
	g_line->SetWidth( 0.5f );

	pd3dDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );

	//////////////////////////////////////////////////////////////////////////

	EpCamera& g_camera = GetG().m_camera;

	// Runtime error at here. I cannot deal with this.. by KYS
	GetG().m_videoMan.SetDev(pd3dDevice);

	//화면의 크기를 변환할 때마다 화면의 크기를 나타내는 전역변수 갱신.
	GetG().m_scrWidth = pBackBufferSurfaceDesc->Width;
	GetG().m_scrHeight = pBackBufferSurfaceDesc->Height;

	//[윤욱]
	//g_menubox.init(pd3dDevice, GetG().m_scrWidth, GetG().m_scrHeight);
	
	V( D3DXCreateFont( pd3dDevice, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulim"), &g_pFont) );

	// Orthogonal and fixed view xforms for GUI or fixed element rendering
	D3DXVECTOR3 eye(0, 0, -50.0f), at(0, 0, 0), up(0, 1.0f, 0);
	D3DXMatrixOrthoLH(&GetG().g_orthoProjMat, (FLOAT)pBackBufferSurfaceDesc->Width, (FLOAT)pBackBufferSurfaceDesc->Height, 0.1f, 100.0f);
	D3DXMatrixLookAtLH(&GetG().g_fixedViewMat,	&eye, &at, &up);

	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
	g_camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 1.0f, 100000.0f );

	
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
    pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
	
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);

	//g_fillColor = D3DCOLOR_ARGB( 0, 45, 50, 170 );
	g_fillColor = D3DCOLOR_ARGB( 0, 0, 0, 0 );

	g_bombShader->onResetDevice();
	g_alphaShader->onResetDevice();
	g_line->OnResetDevice();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;


	//////////////////////////////////////////////////////////////////////////

	GetG().m_camera.FrameMove( fElapsedTime );

	TopStateManager::getSingleton().transit();

	if (GetTopStateManager().getCurState())
		GetTopStateManager().getCurState()->frameMove(fTime, fElapsedTime);

	// Alpha shader testing
	D3DXMATRIXA16 mWorldViewProj;
	D3DXMatrixIdentity( &mWorldViewProj );
	//mWorldViewProj = *GetG().m_camera.GetViewMatrix() * *GetG().m_camera.GetProjMatrix();
	V( g_alphaShader->getConstantTable()->SetMatrix( DXUTGetD3D9Device(), "mWorldViewProj", &mWorldViewProj ) );
	V( g_alphaShader->getConstantTable()->SetFloat( DXUTGetD3D9Device(), "fTime", (float)fTime ) );

}


void renderDebugText()
{
	EpCamera& g_camera = GetG().m_camera;

	WCHAR debugBuffer[512];
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = GetG().m_scrWidth;
	rc.bottom = GetG().m_scrHeight;
	
	StringCchPrintf(debugBuffer, 512, L"カメラの位置: (%.2f, %.2f, %.2f)", g_camera.GetEyePt()->x, g_camera.GetEyePt()->y, g_camera.GetEyePt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"カメラの眺める場所: (%.2f, %.2f, %.2f)", g_camera.GetLookAtPt()->x, g_camera.GetLookAtPt()->y, g_camera.GetLookAtPt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	//StringCchPrintf(debugBuffer, 512, L"タイルの位置: (%.2f, %.2f, %.2f)", g_pic.getPos()->x, g_pic.getPos()->y, g_pic.getPos()->z);
	//g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - 상하좌우 화살표키를 이용해 카메라를 좌우/가까이멀리 이동 가능");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"   (카메라는 항상 +Z 축을 바라보게 됨)");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - I/K/J/L키를 이용해 맵 이미지를 상하좌우로 이동 가능");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - 스페이스키를 이용해 특수기능 작동 (누르고 있으면 필살공격)");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
}

void renderFixedElements(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, &GetG().g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &GetG().g_orthoProjMat);

	//g_menubox.draw(GetG().m_scrWidth, GetG().m_scrHeight);
	//g_picRhw.draw();

	/*D3DPERF_BeginEvent(0xff00ffff, L"Draw Center Smiley~");
	{
		D3DXMATRIX mRot, mScale, mTrans, mWorld;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(45));
		D3DXMatrixScaling(&mScale, 128.0f, 128.0f, 1.0f);
		D3DXMatrixTranslation(&mTrans, 0, 0, -10.0f);
		mWorld = mRot * mScale * mTrans;
		g_picSmiley.setLocalXform(&mWorld);
		g_picSmiley.draw();
	}
	D3DPERF_EndEvent();*/

	renderDebugText();
}

HRESULT drawAlphaAnimatedPlane( double fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;
	GetG().m_dev->SetRenderState( D3DRS_LIGHTING, FALSE );
	GetG().m_dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	GetG().m_dev->SetTexture( 0, 0 );
	V_RETURN( GetG().m_dev->SetVertexShader( g_alphaShader->getVertexShader() ) );
	D3DPERF_BeginEvent( 0, L"Draw Alpha Animated" );
	V_RETURN( g_testPolygonCloned->DrawSubset( 0 ) );
	D3DPERF_EndEvent();
	V_RETURN( GetG().m_dev->SetVertexShader( 0 ) );
	return hr;
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

HRESULT drawTestLines()
{
	//////////////////////////////////////////////////////////////////////////
	const float lineStripHeight = -2.0f;
	const D3DXVECTOR3 lineStrip1[] = {
		D3DXVECTOR3( -0.5f * TileManager::s_tileSize * TileManager::s_xSize,	0,	0 ),
		D3DXVECTOR3( +0.5f * TileManager::s_tileSize * TileManager::s_xSize,	0,	0 ),
	};
	const D3DXVECTOR3 lineStrip2[] = {
		D3DXVECTOR3( 0, -0.5f * TileManager::s_tileSize * TileManager::s_ySize,	0 ),
		D3DXVECTOR3( 0, +0.5f * TileManager::s_tileSize * TileManager::s_ySize,	0 ),
	};
	const D3DCOLOR lineColor = D3DCOLOR_ARGB( 255, 0, 255, 255 );

	g_line->Begin();
	D3DPERF_BeginEvent( 0, L"Line Drawing" );
	D3DXMATRIX mWvp, mWorld, mView, mProj;
	D3DXMATRIX mScale, mTrans;
	mView = *GetG().m_camera.GetViewMatrix();
	mProj = *GetG().m_camera.GetProjMatrix();

	//GetG().m_dev->SetTransform( D3DTS_VIEW, &mView );
	//GetG().m_dev->SetTransform( D3DTS_PROJECTION, &mProj );
	GetG().m_dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	D3DXMatrixScaling( &mScale, 1.0f, 1.0f, 1.0f );

	int i;
	for ( i = -TileManager::s_xSize / 2; i < TileManager::s_xSize / 2; ++i )
	{
		D3DXMatrixTranslation( &mTrans,
			0,
			(float)(TileManager::s_tileSize * i),
			lineStripHeight );

		mWorld = mScale * mTrans;
		mWvp = mWorld * mView * mProj;
		g_line->DrawTransform( lineStrip1, 2, &mWvp, lineColor );

	}
	for ( i = -TileManager::s_ySize / 2; i < TileManager::s_ySize / 2; ++i )
	{
		D3DXMatrixTranslation( &mTrans,
			(float)(TileManager::s_tileSize * i),
			0,
			lineStripHeight );
		mWorld = mScale * mTrans;
		mWvp = mWorld * mView * mProj;
		g_line->DrawTransform( lineStrip2, 2, &mWvp, lineColor );
	}
	D3DPERF_EndEvent();
	g_line->End();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, g_fillColor, 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		drawBurningTeapot( fTime, fElapsedTime );
		GetTopStateManager().getCurState()->frameRender(pd3dDevice, fTime, fElapsedTime);
		drawTestLines();
		drawAlphaAnimatedPlane( fTime, fElapsedTime );

		//////////////////////////////////////////////////////////////////////////
        V( pd3dDevice->EndScene() );
    }

}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	EpCamera& g_camera = GetG().m_camera;

	//g_avatar.handleMessages(hWnd, uMsg, wParam, lParam);
	g_camera.HandleMessages(hWnd, uMsg, wParam, lParam);

	if (g_tsm && GetTopStateManager().getCurState())
		GetTopStateManager().getCurState()->handleMessages(hWnd, uMsg, wParam, lParam);

    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	if ( g_bombShader )
		g_bombShader->onLostDevice();
	if ( g_line )
		g_line->OnLostDevice();
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	SAFE_RELEASE( g_pFont );
	SAFE_RELEASE( g_testTeapot );
	SAFE_RELEASE( g_testPolygon );
	SAFE_RELEASE( g_testPolygonCloned );

	SAFE_DELETE( g_tsm );
	SAFE_DELETE( g_wsm );
	EP_SAFE_RELEASE( g_bombShader );
	EP_SAFE_RELEASE( g_alphaShader );
	EP_SAFE_RELEASE( g_alphaShader );

	SAFE_RELEASE( g_line );

	GetG().m_videoMan.SetDev(0);
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
		case VK_F3:
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		case VK_UP:
			break;
		case VK_DOWN:
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
#else
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
#endif
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
    // Set the callback functions
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
	
    // TODO: Perform any application-level initialization here
	SetCurrentWorkingDirectory();

	// Script Manager Initialization
	CreateScriptManagerIfNotExist();
	GetScriptManager().executeFile( "library/EpInitScript.tcl" );
	GetScriptManager().executeFile( "library/EpWorldState.tcl" );
	GetScriptManager().executeFile( "library/EpDialog1.tcl" );
	//GetScriptManager().executeFile( "library/EpThreadTest.tcl" );

	GetScriptManager().execute( "EpInitApp" );

	GetG().m_camera.SetAttachCameraToModel( true );
	GetG().m_camera.SetEnablePositionMovement( true );


    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, false, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"EmptyProject" );
	DXUTCreateDevice( true, GetG().m_scrWidth, GetG().m_scrHeight );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here
	EP_SAFE_RELEASE( g_scriptManager );
	
    return DXUTGetExitCode();
}

