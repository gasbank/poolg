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
//AlphaShader*					g_alphaShader			= 0;

LPD3DXFONT						g_pFont					= 0;
LPD3DXEFFECT		            g_pEffect				= 0;
D3DXHANDLE						g_tech					= 0;
LPDIRECT3DVERTEXBUFFER9			g_lineElement			= 0;


LPD3DXMESH						g_testTeapot			= 0;
//LPD3DXMESH						g_testPolygon			= 0;
//LPD3DXMESH						g_testPolygonCloned		= 0;

D3DCOLOR						g_fillColor;

LOGMANAGER logMan;

Tcl_Interp*						g_consoleInterp			= 0;
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

	//pDeviceSettings->d3d9.pp.EnableAutoDepthStencil= TRUE;
	//pDeviceSettings->d3d9.pp.AutoDepthStencilFormat = D3DFMT_D16;

	pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_MULTITHREADED;

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
	//pd3dDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );
	//////////////////////////////////////////////////////////////////////////

	// State Manager Initialization
	g_tsm = new TopStateManager();
	g_wsm = new WorldStateManager();

	// Init Script
	GetScriptManager().execute( "EpInitGame" );

	// Shader
	/*
	g_bombShader = new BombShader();
	g_bombShader->initEffect( pd3dDevice, L"Shaders/HLSL/vbomb.fx" );
	g_bombShader->initMainTechnique();
	*/

	//g_alphaShader = new AlphaShader();
	//g_alphaShader->initShader( pd3dDevice, L"Shaders/Alpha.vsh" );
	//g_alphaShader->compileShader( "Alpha", "vs_2_0" );

	D3DXCreateTeapot( pd3dDevice, &g_testTeapot, 0 );
	//D3DXCreatePolygon( pd3dDevice, 0.1f, 32, &g_testPolygon, 0 );

	//g_testPolygon->CloneMesh( 0, g_alphaShader->getDecl(), pd3dDevice, &g_testPolygonCloned );


	V_RETURN( pd3dDevice->CreateVertexBuffer( sizeof( MY_COLOR_VERTEX ) * 2, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &g_lineElement, 0 ) );

	const D3DCOLOR gridColor = D3DCOLOR_ARGB( 255, 32, 128, 128 );
	MY_COLOR_VERTEX data[] = {
		{ -0.5f, 0.0f, -0.2f, gridColor },
		{ +0.5f, 0.0f, -0.2f, gridColor }
	};

	void* pVB = 0;
	V_RETURN( g_lineElement->Lock( 0, 0, &pVB, 0 ) );
	memcpy( pVB, data, sizeof( data ) );
	g_lineElement->Unlock();

	//////////////////////////////////////////////////////////////////////////

	EpCamera& g_camera = GetG().m_camera;

	// Runtime error at here. I cannot deal with this.. by KYS
	GetG().m_videoMan.SetDev(pd3dDevice);

	//화면의 크기를 변환할 때마다 화면의 크기를 나타내는 전역변수 갱신.
	GetG().m_scrWidth = pBackBufferSurfaceDesc->Width;
	GetG().m_scrHeight = pBackBufferSurfaceDesc->Height;

	//[윤욱]
	//g_menubox.init(pd3dDevice, GetG().m_scrWidth, GetG().m_scrHeight);

	V( D3DXCreateFont( pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Courier New"), &g_pFont) );

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

	if ( g_bombShader )
		g_bombShader->onResetDevice();
	//g_alphaShader->onResetDevice();

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;

	UNREFERENCED_PARAMETER( hr );

	//////////////////////////////////////////////////////////////////////////

	GetG().m_camera.frameMove( fElapsedTime );

	TopStateManager::getSingleton().transit();

	if (GetTopStateManager().getCurState())
		GetTopStateManager().getCurState()->frameMove(fTime, fElapsedTime);

	// Alpha shader testing
	D3DXMATRIXA16 mWorldViewProj;
	D3DXMatrixIdentity( &mWorldViewProj );
	//mWorldViewProj = *GetG().m_camera.GetViewMatrix() * *GetG().m_camera.GetProjMatrix();
	//V( g_alphaShader->getConstantTable()->SetMatrix( DXUTGetD3D9Device(), "mWorldViewProj", &mWorldViewProj ) );
	//V( g_alphaShader->getConstantTable()->SetFloat( DXUTGetD3D9Device(), "fTime", (float)fTime ) );
	


}



HRESULT drawAlphaAnimatedPlane( double fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;
	//GetG().m_dev->SetRenderState( D3DRS_LIGHTING, FALSE );
	//GetG().m_dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	//GetG().m_dev->SetTexture( 0, 0 );
	////V_RETURN( GetG().m_dev->SetVertexShader( g_alphaShader->getVertexShader() ) );
	//D3DPERF_BeginEvent( 0, L"Draw Alpha Animated" );
	//V_RETURN( g_testPolygonCloned->DrawSubset( 0 ) );
	//D3DPERF_EndEvent();
	//V_RETURN( GetG().m_dev->SetVertexShader( 0 ) );
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


void renderDebugText()
{
	EpCamera& g_camera = GetG().m_camera;

	WCHAR debugBuffer[512];
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = GetG().m_scrWidth;
	rc.bottom = GetG().m_scrHeight;

	D3DXCOLOR textColor = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );
	StringCchPrintf( debugBuffer, 512, L"Camera Pos     : (%.2f, %.2f, %.2f)", g_camera.GetEyePt()->x, g_camera.GetEyePt()->y, g_camera.GetEyePt()->z );
	g_pFont->DrawTextW( 0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, textColor );
	rc.top += 12;
	StringCchPrintf( debugBuffer, 512, L"Camera LookAt  : (%.2f, %.2f, %.2f)", g_camera.GetLookAtPt()->x, g_camera.GetLookAtPt()->y, g_camera.GetLookAtPt()->z );
	g_pFont->DrawTextW( 0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, textColor );
	rc.top += 12;
	StringCchPrintf( debugBuffer, 512, L"Player Pos     : (%.2f, %.2f, %.2f)", g_camera.GetLookAtPt()->x, g_camera.GetLookAtPt()->y, g_camera.GetLookAtPt()->z );
	g_pFont->DrawTextW( 0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, textColor );
	rc.top += 12;
	StringCchPrintf( debugBuffer, 512, L"Player TilePos : (%.2f, %.2f, %.2f)", g_camera.GetLookAtPt()->x, g_camera.GetLookAtPt()->y, g_camera.GetLookAtPt()->z );
	g_pFont->DrawTextW( 0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, textColor );

}

void renderFixedElements( double fTime, float fElapsedTime )
{
	DXUTGetD3D9Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	DXUTGetD3D9Device()->SetTransform(D3DTS_VIEW, &GetG().g_fixedViewMat);
	DXUTGetD3D9Device()->SetTransform(D3DTS_PROJECTION, &GetG().g_orthoProjMat);

	renderDebugText();
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
		//drawBurningTeapot( fTime, fElapsedTime );

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

		GetTopStateManager().getCurState()->frameRender(pd3dDevice, fTime, fElapsedTime);

		renderFixedElements( fTime, fElapsedTime );
		
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
	g_camera.handleMessages(hWnd, uMsg, wParam, lParam);

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
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	SAFE_RELEASE( g_pFont );
	SAFE_RELEASE( g_testTeapot );
	//SAFE_RELEASE( g_testPolygon );
	//SAFE_RELEASE( g_testPolygonCloned );

	SAFE_DELETE( g_tsm );
	SAFE_DELETE( g_wsm );
	EP_SAFE_RELEASE( g_bombShader );
	//EP_SAFE_RELEASE( g_alphaShader );
	//EP_SAFE_RELEASE( g_alphaShader );

	SAFE_RELEASE( g_lineElement );

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


int EpOutputDebugString( const char* msg )
{
	OutputDebugStringA( msg );
	return 0;

} SCRIPT_CALLABLE_I_PC( EpOutputDebugString )

unsigned int __stdcall newThread( ClientData cd )
{
	//printf( "xx ^________^ xx" );
	g_consoleInterp = Tcl_CreateInterp();
	if (Tcl_Init(g_consoleInterp) == TCL_ERROR)
		return TCL_ERROR;

	GetScriptManager().initBoundings();

	CREATE_OBJ_COMMAND( EpOutputDebugString );


	Tcl_EvalFile( g_consoleInterp, "library/EpThreadTest.tcl" );

	return 0;
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
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
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
	//GetScriptManager().executeFile( "library/EpThreadModuleTest.tcl" );

	GetScriptManager().execute( "EpInitApp" );

	GetG().m_camera.SetAttachCameraToModel( true );
	GetG().m_camera.SetEnablePositionMovement( true );


	Tcl_ThreadId ttid;
	if ( Tcl_CreateThread( &ttid, newThread, 0, 1024, 0 ) != TCL_OK )
		throw std::runtime_error( "Check your Tcl library to support thread" );


	// Initialize DXUT and create the desired Win32 window and Direct3D device for the application
	DXUTInit( true, true ); // Parse the command line and show msgboxes
	DXUTSetHotkeyHandling( true, false, true );  // handle the default hotkeys
	DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
	DXUTCreateWindow( L"EmptyProject" );
	DXUTCreateDevice( true, GetG().m_scrWidth, GetG().m_scrHeight );

	// Start the render loop
	DXUTMainLoop();

	// TODO: Perform any application-level cleanup here
	//Tcl_DeleteInterp( g_consoleInterp );
	EP_SAFE_RELEASE( g_scriptManager );


	return DXUTGetExitCode();
}

