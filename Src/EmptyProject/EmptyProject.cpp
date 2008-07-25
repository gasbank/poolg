//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Modified PoolG Team, a Division of PoolC
//
//--------------------------------------------------------------------------------------
#include "EmptyProjectPCH.h"
#include "DXUTcamera.h"
#include "resource.h"
#include "Picture.h"
#include "Sound.h"

LPDIRECT3DVERTEXSHADER9         g_pVertexShader = NULL;
LPD3DXCONSTANTTABLE             g_pConstantTable = NULL;
LPDIRECT3DVERTEXDECLARATION9    g_pVertexDeclaration = NULL;

LPD3DXEFFECT		            g_pEffect = NULL;       // D3DX effect interface
D3DXHANDLE						g_tech;

CFirstPersonCamera				g_camera;
Picture							g_pic;
Picture							g_avatar;
Sound							g_sound;

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

    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{

	// Setup main camera
	D3DXVECTOR3 vecEye( 0.0f, 0.0f, -5.0f );
	D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
	g_camera.SetViewParams( &vecEye, &vecAt );

	// Load sample image (vertex and index buffer creation with texture)
	g_pic.init(L"tank.jpg", pd3dDevice, 10);
	g_pic.setSize(32, 32);
	g_avatar.init(L"smiley.png", pd3dDevice);
	g_avatar.setSize(1, 1);
	g_sound.init();

	
    return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
	
	g_fillColor = D3DCOLOR_ARGB( 0, 45, 50, 170 );

	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
	g_camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 20.0f );

	//g_tech = g_pEffect->GetTechniqueByName("Main");
	
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	g_pic.frameMove(fElapsedTime);
	g_avatar.frameMove(fElapsedTime);
	g_camera.FrameMove(fElapsedTime);

	//g_pEffect->SetFloat( "gTimer", ( FLOAT )fTime );
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

		// Draw our image
		g_pic.draw();
		
	
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{

	g_avatar.handleMessages(hWnd, uMsg, wParam, lParam);
	g_pic.handleMessages(hWnd, uMsg, wParam, lParam);
	g_camera.HandleMessages(hWnd, uMsg, wParam, lParam);
	g_sound.handleMessages(hWnd, uMsg, wParam, lParam);
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
	g_avatar.release();
	g_sound.release();

	SAFE_RELEASE( g_pVertexShader );
	SAFE_RELEASE( g_pConstantTable );
	SAFE_RELEASE( g_pVertexDeclaration );
	//SAFE_RELEASE( g_pEffect );
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
    DXUTCreateDevice( true, 640, 480 );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}

