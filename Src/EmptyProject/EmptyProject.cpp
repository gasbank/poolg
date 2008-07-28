//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Modified PoolG Team, a Division of PoolC
//
//--------------------------------------------------------------------------------------
#include "EmptyProjectPCH.h"
#include "resource.h"
#include "Picture.h"
#include "PictureMap.h"
#include "Sound.h"
#include "CJW.h"
#include "IntroModule.h"

enum { GAMESTATE_INTRO, GAMESTATE_MENU, GAMESTATE_WORLD, GAMESTATE_FIGHT };

int								g_CurrentState = GAMESTATE_INTRO;

LPDIRECT3DVERTEXSHADER9         g_pVertexShader = NULL;
LPD3DXCONSTANTTABLE             g_pConstantTable = NULL;
LPDIRECT3DVERTEXDECLARATION9    g_pVertexDeclaration = NULL;
LPD3DXFONT						g_pFont = NULL;

LPD3DXEFFECT		            g_pEffect = NULL;       // D3DX effect interface
D3DXHANDLE						g_tech;

CFirstPersonCamera				g_camera;
PictureMap						g_pic;
Picture							g_picRhw;
Picture							g_avatar;
Sound							g_sound;
CJW								g_jw;
IntroModule						g_introModule;

D3DCOLOR						g_fillColor;

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
	pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
	pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
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

	// Setup main camera
	D3DXVECTOR3 vecEye( 0.0f, 0.0f, -30.0f );
	D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
	g_camera.SetViewParams( &vecEye, &vecAt );

	// Load sample image (vertex and index buffer creation with texture)
	const UINT mapSegments = 16;
	const UINT mapSize = 12;
	g_pic.init(L"graytile.jpg", pd3dDevice, mapSegments);
	g_pic.setSize(12, 12);
	g_pic.setPos(D3DXVECTOR3(g_pic.getPos()->x - mapSize/2, g_pic.getPos()->y - mapSize/2, g_pic.getPos()->z));
	g_picRhw.initRhw(L"title.jpg", pd3dDevice);

	// Create 3D Mesh Texts for intro cinema.
	g_introModule.CreateTextMeshes( pd3dDevice );

	//[재우] 부분
	g_jw.init(pd3dDevice);
	
	g_avatar.init(L"smiley.png", pd3dDevice);
	g_avatar.setSize(1, 1);
	g_sound.init();

	V( D3DXCreateFont( pd3dDevice, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulim"), &g_pFont) );

	// Create vertex shader
	WCHAR strPath[512];
	LPD3DXBUFFER pCode;

	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	V_RETURN( pd3dDevice->CreateVertexDeclaration( decl, &g_pVertexDeclaration ) );

	V_RETURN(StringCchCopy( strPath, sizeof(strPath)/sizeof(TCHAR), L"HLSLwithoutEffects.vsh" ));

	DWORD dwShaderFlags = D3DXSHADER_DEBUG | D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
	V_RETURN( D3DXCompileShaderFromFile( strPath, NULL, NULL, "Ripple",
		"vs_2_0", dwShaderFlags, &pCode,
		NULL, &g_pConstantTable ) );

	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader( ( DWORD* )pCode->GetBufferPointer(),
		&g_pVertexShader );
	pCode->Release();
	
	if( FAILED( hr ) )
		return DXTRACE_ERR( TEXT( "CreateVertexShader" ), hr );
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
	switch ( g_CurrentState )
	{
	case GAMESTATE_INTRO:
		g_introModule.SetCameraAndLight( pd3dDevice, pBackBufferSurfaceDesc, &g_camera );				
		break;

	default:
		D3DXVECTOR3 vecEye( 0.0f, 0.0f, -30.0f );
		D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
		g_camera.SetViewParams( &vecEye, &vecAt );				

		g_fillColor = D3DCOLOR_ARGB( 0, 45, 50, 170 );

		pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
		g_camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 100.0f );
	}
	
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	switch ( g_CurrentState )
	{
	case GAMESTATE_INTRO:
		g_introModule.frameMove(fTime);
		break;
	default:
		g_pic.frameMove(fElapsedTime);
		g_avatar.frameMove(fElapsedTime);
		g_camera.FrameMove(fElapsedTime);

		//[재우]부분
		g_jw.frameMove(fElapsedTime);


		// Set up the vertex shader constants
		D3DXMATRIXA16 mViewProj = *g_camera.GetViewMatrix() * *g_camera.GetProjMatrix();
		g_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mViewProj", &mViewProj );
		g_pConstantTable->SetFloat( DXUTGetD3D9Device(), "fTime", ( float )fTime );
	}
}


void renderDebugText()
{
	WCHAR debugBuffer[512];
	RECT rc;

	SetRect(&rc, 0, 0, 0, 0);
	
	StringCchPrintf(debugBuffer, 512, L"カメラの位置: (%.2f, %.2f, %.2f)", g_camera.GetEyePt()->x, g_camera.GetEyePt()->y, g_camera.GetEyePt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"カメラの眺める場所: (%.2f, %.2f, %.2f)", g_camera.GetLookAtPt()->x, g_camera.GetLookAtPt()->y, g_camera.GetLookAtPt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"タイルの位置: (%.2f, %.2f, %.2f)", g_pic.getPos()->x, g_pic.getPos()->y, g_pic.getPos()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - 상하좌우 화살표키를 이용해 카메라를 좌우/가까이멀리 이동 가능");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"   (카메라는 항상 +Z 축을 바라보게 됨)");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - I/K/J/L키를 이용해 맵 이미지를 상하좌우로 이동 가능");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - 스페이스키를 이용해 특수기능 작동 (누르고 있으면 필살공격)");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
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
		// Setup view and projection xforms
		pd3dDevice->SetTransform(D3DTS_VIEW, g_camera.GetViewMatrix());
		pd3dDevice->SetTransform(D3DTS_PROJECTION, g_camera.GetProjMatrix());

		switch ( g_CurrentState )
		{
		case GAMESTATE_INTRO:
			g_introModule.draw(pd3dDevice);
			break;
		default:

			// Draw picture map with shader settings
			D3DXMATRIXA16 mWorldViewProj = *g_pic.getLocalXform() * *g_camera.GetViewMatrix() * *g_camera.GetProjMatrix();
			g_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mWorldViewProj", &mWorldViewProj );

			//pd3dDevice->SetVertexDeclaration( g_pVertexDeclaration );
			//pd3dDevice->SetVertexShader( g_pVertexShader )//;
			

			//[재우]잠시 이 부분을 지우겠습니다.
		//	g_pic.draw();
			g_picRhw.draw();

			//[재우]부분
			g_jw.draw();

			renderDebugText();
		}
			
		
	
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	switch ( g_CurrentState )
	{
	case GAMESTATE_INTRO:
		break;
	default:
		//g_avatar.handleMessages(hWnd, uMsg, wParam, lParam);
		g_pic.handleMessages(hWnd, uMsg, wParam, lParam);
		g_camera.HandleMessages(hWnd, uMsg, wParam, lParam);
		g_sound.handleMessages(hWnd, uMsg, wParam, lParam);
		g_sound.UpdateAudio();

		//[재우]부분
		g_jw.handleMessages(hWnd, uMsg, wParam, lParam);
	}

    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	g_pic.release();
	g_picRhw.release();
	g_avatar.release();
	g_sound.release();

	//[재우]부분
	g_jw.release();
	
	g_introModule.release();

	SAFE_RELEASE( g_pVertexShader );
	SAFE_RELEASE( g_pConstantTable );
	SAFE_RELEASE( g_pVertexDeclaration );
	//SAFE_RELEASE( g_pEffect );
	SAFE_RELEASE( g_pFont );
	
}


//--------------------------------------------------------------------------------------
// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	switch ( g_CurrentState )
	{
	case GAMESTATE_INTRO:
		if ( bKeyDown )
			// F4를 누르면 인트로에서 메인 화면으로 넘어간다.
			// 웃기지만 DXUTToggleFullScreen()을 호출하면 동시에 OnD3D9ResetDevice가
			// 호출되어 카메라 및 조명을 초기화해주기 때문에, 빠르게 두번 호출한다. 하하.
			// by 영신.
			if ( nChar == VK_F4 )
			{
				g_CurrentState = GAMESTATE_MENU;				
				DXUTToggleFullScreen();
				DXUTToggleFullScreen();
			}
		break;
	default:
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
			case VK_F2:
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
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Setup working directory
	TCHAR buf[MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	GetModuleFileName(NULL, buf, MAX_PATH);
	_tsplitpath_s(buf, drive, _MAX_DRIVE, dir, _MAX_DIR, 0, 0, 0, 0);
	StringCchPrintf(buf, MAX_PATH, _T("%s%s"), drive, dir);
	SetCurrentDirectory(buf);

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

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"EmptyProject" );
    DXUTCreateDevice( true, 1024, 768 );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}

