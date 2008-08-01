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
#include "Picture.h"
#include "PictureMap.h"
#include "Sound.h"
#include "Battle.h"
#include "IntroModule.h"
#include "Unit.h"
#include "EpCamera.h"
#include "Menu.h"
#include "ArnFile.h"
#include "ArnSceneGraph.h"
#include "ArnNode.h"
#include "VideoMan.h"

enum GameTopState { GAMESTATE_INTRO, GAMESTATE_WORLD, GAMESTATE_FIGHT };

GameTopState					g_CurrentState			= GAMESTATE_INTRO;

bool							g_IntroEnable = true;
bool							g_WorldEnable = false;
double							g_timeDelta = 0.0f;

LPDIRECT3DVERTEXSHADER9         g_pVertexShader			= 0;
LPD3DXCONSTANTTABLE             g_pConstantTable		= 0;
LPDIRECT3DVERTEXDECLARATION9    g_pVertexDeclaration	= 0;
LPD3DXFONT						g_pFont					= 0;
LPD3DXEFFECT		            g_pEffect				= 0;
D3DXHANDLE						g_tech					= 0;

Unit							g_sampleTeapotMesh;
float							g_sampleTeapotMeshRot	= 0;
LPD3DXMESH						g_aTile					= 0;
D3DLIGHT9						g_light;

EpCamera						g_camera;
PictureMap						g_pic;
Picture							g_picRhw;
Picture							g_picSmiley;
Picture							g_avatar;
Menu							g_menubox;
Menu							g_select;
Sound							g_sound;
Battle							g_battle;
IntroModule						g_introModule;

D3DCOLOR						g_fillColor;

D3DXMATRIX						g_orthoProjMat;
D3DXMATRIX						g_fixedViewMat;

int						g_scrWidth				= 160 * 5;
int						g_scrHeight				= 90 * 5;

VideoMan videoMan;
LOGMANAGER logMan;

ArnFileData*					g_afd;
ArnSceneGraph*					g_sg;

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

	VideoMan::getSingleton().SetDev(pd3dDevice);
	// Aran file init
	g_afd = new ArnFileData;
	load_arnfile(_T("gus2.arn"), *g_afd);
	g_sg = new ArnSceneGraph(*g_afd);

	//화면의 크기를 변환할 때마다 화면의 크기를 나타내는 전역변수 갱신.
	g_scrWidth = pBackBufferSurfaceDesc->Width;
	g_scrHeight = pBackBufferSurfaceDesc->Height;

	// Setup main camera
	D3DXVECTOR3 vecEye( 0.0f, 0.0f, -30.0f );
	D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
	g_camera.SetViewParams( &vecEye, &vecAt );

	// Load sample image (vertex and index buffer creation with texture)
	const UINT mapSegments = 32;
	const UINT mapSize = 32;
	g_pic.init(L"graytile.tga", pd3dDevice, mapSegments);
	g_pic.setSize((float)mapSize, (float)mapSize);
	g_pic.setPos(D3DXVECTOR3(g_pic.getPos()->x - mapSize/2, g_pic.getPos()->y - mapSize/2, g_pic.getPos()->z));
	
	g_picRhw.initRhw(L"smiley.png", pd3dDevice);
	g_picSmiley.init(L"smiley.png", pd3dDevice);
	//g_picSmiley.setSizeToTexture();

	//[윤욱]
	g_menubox.init(pd3dDevice, g_scrWidth, g_scrHeight);


	// Create 3D Mesh Texts for intro cinema.
	g_introModule.CreateTextMeshes( pd3dDevice );

	//[재우] 부분
	g_battle.init(pd3dDevice, g_scrWidth, g_scrHeight);
	
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


	// Orthogonal and fixed view xforms for GUI or fixed element rendering
	D3DXVECTOR3 eye(0, 0, -50.0f), at(0, 0, 0), up(0, 1.0f, 0);
	D3DXMatrixOrthoLH(&g_orthoProjMat, (FLOAT)pBackBufferSurfaceDesc->Width, (FLOAT)pBackBufferSurfaceDesc->Height, 0.1f, 100.0f);
	D3DXMatrixLookAtLH(&g_fixedViewMat,	&eye, &at, &up);

	// Create sample 3D model(!)
	LPD3DXMESH teapot;
	D3DXCreateTeapot(pd3dDevice, &teapot, 0);
	g_sampleTeapotMesh.init(pd3dDevice, teapot);
	g_sampleTeapotMesh.setPosZ(-g_sampleTeapotMesh.getUpperRight().z);
	g_sampleTeapotMesh.setRotX(D3DXToRadian(-90));
	g_sampleTeapotMesh.setRotZ(D3DXToRadian(90));
	
	D3DXCreateBox(pd3dDevice, 1.0f, 1.0f, 1.0f, &g_aTile, 0);

	ZeroMemory(&g_light, sizeof(D3DLIGHT9));
	D3DCOLORVALUE cv = { 0.8f, 0.8f, 0.8f, 1.0f };
	g_light.Ambient = cv;
	g_light.Diffuse = cv;
	g_light.Specular = cv;
	/*
	g_light.Attenuation0 = 0.5f;
	g_light.Attenuation1 = 0.0f;
	g_light.Attenuation2 = 0.0f;
	*/
	
	D3DXVECTOR3 dir(10.0f, -10.0f, 10.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_light.Direction, &dir);
	g_light.Falloff = 0.5f;
	g_light.Phi = D3DXToRadian(80);
	g_light.Theta = D3DXToRadian(10);
	
	D3DXVECTOR3 pos(-10.0f, 10.0f, -10.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_light.Position, &pos);

	g_light.Type = D3DLIGHT_DIRECTIONAL;
	g_light.Range = 1000.0f;
	
	
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

	pd3dDevice->SetLight(0, &g_light);
	pd3dDevice->LightEnable(0, TRUE);

	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
	g_camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 1.0f, 1000.0f );

		
	
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime_, float fElapsedTime, void* pUserContext )
{
	double fTime = fTime_ + g_timeDelta;

	if ( fTime < 41.0f )
	{
		g_introModule.frameMove(fTime);

		// Setup the camera with view & projection matrix
		// for intro cinema.
		D3DXVECTOR3 vecEye( 0.0f, -30.0f, -20.0f );
		D3DXVECTOR3 vecAt( 0.0f, 0.0f, 0.0f );
		g_camera.SetViewParams( &vecEye, &vecAt );
	}

	if ( 41.0f < fTime && fTime < 44.0f)
	{
		g_IntroEnable = false;
		g_CurrentState = GAMESTATE_WORLD;
	}

	
	if ( 36.0f < fTime && fTime < 41.0f )
	{
		float newfTime = ( float )fTime - 36.0f;

		D3DXVECTOR3 vecEye( 
			0.0f, 
			(-30.0f * (5.0f - newfTime) + 0.0f * newfTime) / 5.0f, 
			(-20.0f * (5.0f - newfTime) -20.0f * newfTime) / 5.0f );
		D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
		g_camera.SetViewParams( &vecEye, &vecAt );
	
		D3DCOLORVALUE cv = { newfTime / 5.0f , newfTime / 5.0f, newfTime / 5.0f, newfTime / 5.0f };
		const float lightLimit = 0.2f;
		if (cv.r > lightLimit) cv.r = lightLimit;
		if (cv.g > lightLimit) cv.g = lightLimit;
		if (cv.b > lightLimit) cv.b = lightLimit;
		g_light.Ambient = cv;
	}

	if ( 36.0f < fTime )
	{
		g_WorldEnable = true;

		g_pic.frameMove(fElapsedTime);
		g_avatar.frameMove(fElapsedTime);
		g_camera.FrameMove(fElapsedTime);
		g_sound.UpdateAudio();
		g_sampleTeapotMesh.frameMove(fElapsedTime);

		//[재우]부분
		g_battle.frameMove(fElapsedTime);
		g_menubox.frameMove(fElapsedTime);

		// Set up the vertex shader constants
		D3DXMATRIXA16 mViewProj = *g_camera.GetViewMatrix() * *g_camera.GetProjMatrix();
		g_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mViewProj", &mViewProj );
		g_pConstantTable->SetFloat( DXUTGetD3D9Device(), "fTime", ( float )fTime );

		g_sampleTeapotMeshRot += fElapsedTime * D3DXToRadian(35); // 35 degrees per second

		
	}
}


void renderDebugText()
{
	WCHAR debugBuffer[512];
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = g_scrWidth;
	rc.bottom = g_scrHeight;
	
	StringCchPrintf(debugBuffer, 512, L"カメラの位置: (%.2f, %.2f, %.2f)", g_camera.GetEyePt()->x, g_camera.GetEyePt()->y, g_camera.GetEyePt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"カメラの眺める場所: (%.2f, %.2f, %.2f)", g_camera.GetLookAtPt()->x, g_camera.GetLookAtPt()->y, g_camera.GetLookAtPt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"タイルの位置: (%.2f, %.2f, %.2f)", g_pic.getPos()->x, g_pic.getPos()->y, g_pic.getPos()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
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

	

	//[재우]부분
	g_battle.draw(g_scrWidth, g_scrHeight);

	g_menubox.draw(g_scrWidth, g_scrHeight);
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

		if ( g_IntroEnable )
			g_introModule.draw(pd3dDevice, &g_camera);

		if ( g_WorldEnable )
		{
			pd3dDevice->SetLight(0, &g_light);

			pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//////////////////////////////////////////////////////////////////////////
			// Perspective Rendering Phase

			pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			pd3dDevice->SetTransform(D3DTS_VIEW, g_camera.GetViewMatrix());
			pd3dDevice->SetTransform(D3DTS_PROJECTION, g_camera.GetProjMatrix());

			// Draw picture map with shader settings
			D3DXMATRIXA16 mWorldViewProj = *g_pic.getLocalXform() * *g_camera.GetViewMatrix() * *g_camera.GetProjMatrix();
			g_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mWorldViewProj", &mWorldViewProj );
			
			
			// Sample 3D model rendering
			g_sampleTeapotMesh.draw();

			// Draw floor gray tile (2D)
			//g_pic.draw();

			D3DXMATRIX transform;
			D3DXMatrixIdentity(&transform);
			pd3dDevice->SetTransform(D3DTS_WORLD, &transform);

			pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			pd3dDevice->SetFVF(ArnVertex::FVF);
			videoMan.renderMeshesOnly(g_sg->getSceneRoot());
			g_sg->getSceneRoot()->update(fTime, fElapsedTime);
			


			//////////////////////////////////////////////////////////////////////////
			// Orthogonal and fixed Rendering Phase

			renderFixedElements(pd3dDevice, fTime, fElapsedTime);
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
	//g_avatar.handleMessages(hWnd, uMsg, wParam, lParam);
	g_pic.handleMessages(hWnd, uMsg, wParam, lParam);
	g_camera.HandleMessages(hWnd, uMsg, wParam, lParam);
	g_sound.handleMessages(hWnd, uMsg, wParam, lParam);
	g_sampleTeapotMesh.handleMessages(hWnd, uMsg, wParam, lParam);

	//[재우]부분
	g_battle.handleMessages(hWnd, uMsg, wParam, lParam);

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
	g_picSmiley.release();
	g_avatar.release();
	g_sound.release();
	g_menubox.release();

	//[재우]부분
	g_battle.release();

	g_introModule.release();
	g_sampleTeapotMesh.release();

	SAFE_RELEASE( g_pVertexShader );
	SAFE_RELEASE( g_pConstantTable );
	SAFE_RELEASE( g_pVertexDeclaration );
	//SAFE_RELEASE( g_pEffect );
	SAFE_RELEASE( g_pFont );
	SAFE_RELEASE(g_aTile);

	release_arnfile(*g_afd);
	delete g_afd;
	delete g_sg;
	VideoMan::getSingleton().SetDev(0);
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
			if ( nChar == VK_F4 )
			{
				g_timeDelta += 1.0f;
			}
		break;
	case GAMESTATE_WORLD:
		if ( bKeyDown )
		{
		}
		break;
	}

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
	//[윤욱]
	if( bKeyDown )
	{
		switch( nChar )
		{
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
    DXUTCreateDevice( true, g_scrWidth, g_scrHeight );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}

