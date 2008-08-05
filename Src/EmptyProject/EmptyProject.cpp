﻿//--------------------------------------------------------------------------------------
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
#include "Menu.h"
#include "StateManager.h"
#include "VideoMan.h"

G								g_g;
StateManager					g_sm;

double							g_timeDelta = 0.0f;

LPD3DXFONT						g_pFont					= 0;
LPD3DXEFFECT		            g_pEffect				= 0;
D3DXHANDLE						g_tech					= 0;

Menu							g_menubox;
Menu							g_select;

D3DCOLOR						g_fillColor;

D3DXMATRIX						g_orthoProjMat;
D3DXMATRIX						g_fixedViewMat;

LOGMANAGER logMan;




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


int square (int i)
{
	return i*i;
}
int csum(int a, int b)
{
	return a+b;
}
int EpSetNextState(int stateID)
{
	StateManager::getSingleton().setNextState((GameTopState)stateID);
	return 0;
}
static int _wrap_csum(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	int  _result;
	int  _arg0, _arg1;
	Tcl_Obj * tcl_result;
	int tempint1, tempint2;

	clientData = clientData; objv = objv;
	tcl_result = Tcl_GetObjResult(interp);
	if ((objc < 3) || (objc > 3)) {
		Tcl_SetStringObj(tcl_result,"Wrong # args. csum a b ",-1);
		return TCL_ERROR;
	}
	if (Tcl_GetIntFromObj(interp,objv[1],&tempint1) == TCL_ERROR) return TCL_ERROR;
	if (Tcl_GetIntFromObj(interp,objv[2],&tempint2) == TCL_ERROR) return TCL_ERROR;
	_arg0 = (int ) tempint1;
	_arg1 = (int ) tempint2;
	_result = (int )csum(_arg0, _arg1);
	tcl_result = Tcl_GetObjResult(interp);
	Tcl_SetIntObj(tcl_result,(long) _result);
	return TCL_OK;
}

static int _wrap_square(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	int  _result;
	int  _arg0;
	Tcl_Obj * tcl_result;
	int tempint;

	clientData = clientData; objv = objv;
	tcl_result = Tcl_GetObjResult(interp);
	if ((objc < 2) || (objc > 2)) {
		Tcl_SetStringObj(tcl_result,"Wrong # args. square i ",-1);
		return TCL_ERROR;
	}
	if (Tcl_GetIntFromObj(interp,objv[1],&tempint) == TCL_ERROR) return TCL_ERROR;
	_arg0 = (int ) tempint;
	_result = (int )square(_arg0);
	tcl_result = Tcl_GetObjResult(interp);
	Tcl_SetIntObj(tcl_result,(long) _result);
	return TCL_OK;
}
static int _wrap_EpSetNextState(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	int  _result;
	int  _arg0;
	Tcl_Obj * tcl_result;
	int tempint;

	clientData = clientData; objv = objv;
	tcl_result = Tcl_GetObjResult(interp);
	if ((objc < 2) || (objc > 2)) {
		Tcl_SetStringObj(tcl_result,"Wrong # args. square i ",-1);
		return TCL_ERROR;
	}
	if (Tcl_GetIntFromObj(interp,objv[1],&tempint) == TCL_ERROR) return TCL_ERROR;
	_arg0 = (int ) tempint;
	_result = (int )EpSetNextState(_arg0);
	tcl_result = Tcl_GetObjResult(interp);
	Tcl_SetIntObj(tcl_result,(long) _result);
	return TCL_OK;
}

int Tcl_AppInit(Tcl_Interp *interp){
	if (Tcl_Init(interp) == TCL_ERROR)
		return TCL_ERROR;
	/* Now initialize our functions */
	Tcl_CreateObjCommand(interp, "square", _wrap_square, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "csum", _wrap_csum, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "EpSetNextState", _wrap_EpSetNextState, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	return TCL_OK;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


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

	G::getSingleton().m_dev = pd3dDevice;
	EpCamera& g_camera = G::getSingleton().m_camera;

	// Runtime error at here. I cannot deal with this.. by KYS
	G::getSingleton().m_videoMan.SetDev(pd3dDevice);

	//화면의 크기를 변환할 때마다 화면의 크기를 나타내는 전역변수 갱신.
	G::getSingleton().m_scrWidth = pBackBufferSurfaceDesc->Width;
	G::getSingleton().m_scrHeight = pBackBufferSurfaceDesc->Height;

	//[윤욱]
	g_menubox.init(pd3dDevice, G::getSingleton().m_scrWidth, G::getSingleton().m_scrHeight);
	
	V( D3DXCreateFont( pd3dDevice, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulim"), &g_pFont) );

	// Orthogonal and fixed view xforms for GUI or fixed element rendering
	D3DXVECTOR3 eye(0, 0, -50.0f), at(0, 0, 0), up(0, 1.0f, 0);
	D3DXMatrixOrthoLH(&g_orthoProjMat, (FLOAT)pBackBufferSurfaceDesc->Width, (FLOAT)pBackBufferSurfaceDesc->Height, 0.1f, 100.0f);
	D3DXMatrixLookAtLH(&g_fixedViewMat,	&eye, &at, &up);

	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
	g_camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 1.0f, 1000.0f );

	
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

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime_, float fElapsedTime, void* pUserContext )
{
	double fTime = fTime_ + g_timeDelta;

	g_menubox.frameMove(fElapsedTime);

	if (g_sm.getCurState())
		g_sm.getCurState()->frameMove(fTime, fElapsedTime);

	StateManager::getSingleton().transit();
}


void renderDebugText()
{
	EpCamera& g_camera = G::getSingleton().m_camera;

	WCHAR debugBuffer[512];
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = G::getSingleton().m_scrWidth;
	rc.bottom = G::getSingleton().m_scrHeight;
	
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
	pd3dDevice->SetTransform(D3DTS_VIEW, &g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &g_orthoProjMat);

	g_menubox.draw(G::getSingleton().m_scrWidth, G::getSingleton().m_scrHeight);
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
		g_sm.getCurState()->frameRender(pd3dDevice, fTime, fElapsedTime);			


		//////////////////////////////////////////////////////////////////////////
		// Orthogonal and fixed Rendering Phase

		//renderFixedElements(pd3dDevice, fTime, fElapsedTime);

        V( pd3dDevice->EndScene() );
    }

}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	EpCamera& g_camera = G::getSingleton().m_camera;

	//g_avatar.handleMessages(hWnd, uMsg, wParam, lParam);
	g_camera.HandleMessages(hWnd, uMsg, wParam, lParam);

	if (g_sm.getCurState() != 0)
		g_sm.getCurState()->handleMessages(hWnd, uMsg, wParam, lParam);

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
	g_menubox.release();
	
	g_sm.release();
	
	//SAFE_RELEASE( g_pEffect );
	SAFE_RELEASE( g_pFont );
	
	G::getSingleton().m_videoMan.SetDev(0);
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
		case '8': g_select.select(8);
			break;
		case '9': g_select.select(9);
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
	// Setup working directory
	TCHAR buf[MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	GetModuleFileName(NULL, buf, MAX_PATH);
	_tsplitpath_s(buf, drive, _MAX_DRIVE, dir, _MAX_DIR, 0, 0, 0, 0);
	StringCchPrintf(buf, MAX_PATH, _T("%s%s"), drive, dir);
	SetCurrentDirectory(buf);

	g_sm.init();

	Tcl_Interp* interp;
	interp = Tcl_CreateInterp();
	Tcl_AppInit(interp);
	assert(Tcl_EvalFile( interp, "library/EpInitScript.tcl" ) == TCL_OK) ;
	assert(Tcl_Eval(interp, "EpInitGame") == TCL_OK);

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"EmptyProject" );
	DXUTCreateDevice( true, G::getSingleton().m_scrWidth, G::getSingleton().m_scrHeight );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here
	Tcl_DeleteInterp(interp);
	Tcl_Finalize();

    return DXUTGetExitCode();
}

