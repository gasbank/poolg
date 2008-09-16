/**
	@file EmptyProject.cpp
	@brief main()/WinMain() 함수가 있는 파일입니다.
	EP 전반에서 쓰이는 전역 변수의 할당/초기화/해제를 담당하고 있습니다.
	@details
	EP는 DXUT(DirectX Utility Kit) 소스 코드를 기반으로 작성되었습니다.
	DXUT는 DirectX 라이브러리를 좀 더 편리하게 사용할 수 있도록 도와주는 랩퍼 라이브러리라고
	볼 수 있습니다.
	DXUT는 화면 출력이나 입력, UI, 카메라 관련 클래스 및 비디오 게임에서 자주 사용되는
	아주 많은 요소에 대해 다양한 프레임워크와 클래스를 제공하지만 EP에서 사용된
	주된 기능은 화면 출력을 위한 프레임워크 및 카메라 클래스입니다. 이외의 기능은 사용하지
	않았습니다.

	본 프로그램이 처음으로 실행되었을 상태부터 끝날때까지의 함수 호출 순서는 다음과 같습니다.

		-# WinMain
		-# OnCreateDevice
		-# OnResetDevice
		-# OnFrameMove / OnFrameRender loop (주 루프)
		-# OnLostDevice
		-# OnDestroyDevice
*/

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
#include "Skill.h"
#include "SkillManager.h"

#include "EpReplicaManagerConnection.h"
#include "EpUser.h"



G									g_g;
WorldManager*						g_wm							= 0;
TopStateManager*					g_tsm							= 0;
WorldStateManager*					g_wsm							= 0;
ScriptManager*						g_scriptManager					= 0;		// Set to zero is 'CRUCIAL!'
SkillManager*						g_skillManager					= 0;
SpriteManager*						g_spriteManager					= 0;
EpLight*							g_epLight						= 0;
SCREEN_VERTEX						g_Vertex[4];

LPD3DXEFFECT						g_pEffect						= 0;
D3DXHANDLE							g_tech							= 0;
LPDIRECT3DVERTEXBUFFER9				g_lineElement					= 0;
HANDLE								g_scriptBindingFinishedEvent	= 0;		// Signal object to resolve multi-threaded problems on console thread and main app thread
HANDLE								g_consoleReleasedEvent			= 0;		// Signal object to resolve multi-threaded problems on console thread and main app thread

LPD3DXMESH							g_bst[BST_COUNT];

D3DCOLOR							g_fillColor;


// LPD3DXFONTs will be manag	ed at global scope.

LPD3DXFONT							g_pFont							= 0;
LPD3DXFONT							g_unitNameFont					= 0;
LPD3DXFONT							g_dlgNameFont					= 0;
LPD3DXFONT							g_dlgContentFont				= 0;
// CreditState
LPD3DXFONT							g_d3dxFont						= 0;
LPD3DXFONT							g_d3dxFontBig					= 0;
// Battle State
LPD3DXFONT							g_fontBattle2					= 0;
LPD3DXFONT							g_fontBattle					= 0;
LPD3DXFONT							g_fontSkill						= 0;
LPD3DXFONT							g_fontSkillDescription			= 0;
LPD3DXFONT							g_fontStat						= 0;


LOGMANAGER							logMan;

Tcl_Interp*							g_consoleInterp					= 0;

std::wstring						g_debugBuffer;
bool								g_bTileGrid						= false;


LPDIRECT3DTEXTURE9					g_pFullScreenRenderTarget		= 0;
LPDIRECT3DSURFACE9					g_pFullScreenRenderTargetSurf	= 0;

LPDIRECT3DTEXTURE9					g_sepiaRenderTarget				= 0;
LPDIRECT3DSURFACE9					g_sepiaRenderTargetSurf			= 0;

LPDIRECT3DTEXTURE9					g_radialBlurRenderTarget		= 0;
LPDIRECT3DSURFACE9					g_radialBlurRenderTargetSurf	= 0;

PostSepiaShader*					g_postSepiaShader				= 0;
PostRadialBlurShader*				g_postRadialBlurShader			= 0;
BombShader*							g_bombShader					= 0;

// Particle System related
int									g_nActiveSystem					= 0;
CParticleSystem*					g_pParticleSystems[6];
bool								g_bParticleVisible				= false;

// RakNet
RakPeerInterface*					g_clientPeer					= 0; ///< RakNet Client Peer
RakNet::RPC3						g_rpc3Inst;
NetworkIDManager					g_networkIDManager;
// The system that performs most of our functionality for this demo
RakNet::ReplicaManager2*			g_replicaManager				= 0;
// Instance of the class that creates the object we use to represent connections
EpReplicaManagerConnectionFactory	g_connectionFactory;

// Called from EpCommonLibrary
NetworkIDManager& GetNetworkIdManager() { return g_networkIDManager; }
RakPeerInterface* GetRakPeer() { return g_clientPeer; }
bool QueryIsNetworkServer() { return false; }

void AddToCurrentWorld( UnitBase* unitBase )
{
	GetWorldManager().getCurWorld()->addUnit( unitBase );
}
//////////////////////////////////////////////////////////////////////////

void ConfigureShaders( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
void ConfigureParticleSystem( LPDIRECT3DDEVICE9 pd3dDevice );
void ConfigureTileGridGeometry( LPDIRECT3DDEVICE9 pd3dDevice );
void ConfigureTestGeometry( LPDIRECT3DDEVICE9 pd3dDevice );

void SetupFullscreenQuad( const D3DSURFACE_DESC* pBackBufferSurfaceDesc );

void ConnectToServer();
void DisconnectFromServer();



void PrintHelloWorld( int number )
{
	odprintf( "Hello World from RPC!: %d\n", number );
}

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


/*!
	\brief 디바이스 리셋 이벤트에도 살아남는 개체를 생성합니다.
	\param pd3dDevice D3D9 디바이스
	\param pBackBufferSurfaceDesc 백 버퍼 서피스 설정값을 참조하기 위한 포인터
	\param pUserContext 사용하지 않는 매개변수
	\details
	DXUT에서 정의된 콜백 함수입니다.

	본 함수는 D3D9 디바이스가 얻어진 이후 실행됩니다. 이 함수에서는 디바이스 리셋 이벤트와
	상관없이 항상(디바이스 로스트 전까지) 유효한 D3D 개체를 생성하게 됩니다. 일반적으로 디바이스
	리셋 이벤트가 발생한 경우 D3D9 디바이스가 재생성되기 때문에 이에 의존적인 모든 개체를 다시 생성하거나
	갱신해야만 합니다. 그러나 D3DPOOL_MANAGED 플래그로 생성된 D3DX 관련 개체는 디바이스 리셋 이벤트시
	Direct3D가 자동으로 개체의 갱신을 책임지게 됩니다. 이렇듯 디바이스 리셋 이벤트에도
	'살아남는' 개체의 경우 본 함수에서 생성하도록 합니다. 디바이스 리셋 이벤트에서
	'살아남지 못하는' 개체의 할당 및 초기화는 OnD3D9ResetDevice()에서 호출해 주십시오.

	참고로 처음 DXUT 소스 코드에는 아래와 같은 커맨트가 있었습니다.

	Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
	and aren't tied to the back buffer size

	이후는 본 함수에서 일어나는 일 중 중요한 것을 나열한 것입니다.
*/
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									void* pUserContext )
{
	OutputDebugString( _T( " - INFO: OnCreateDevice called.\n" ) );

	HRESULT hr = S_OK;

	UNREFERENCED_PARAMETER( hr );


	/// - Aran 라이브러리의 D3D9 디바이스 값을 설정합니다.
	assert( VideoMan::getSingleton().GetDev() == 0 );
	VideoMan::getSingleton().SetDev( pd3dDevice );
	/// - 화면 해상도 값을 전역 변수 객체인 GetG()에 설정합니다.
	GetG().m_scrWidth = pBackBufferSurfaceDesc->Width;
	GetG().m_scrHeight = pBackBufferSurfaceDesc->Height;
	
	/// - GUI 요소나 고정된 2D 그래픽을 출력하기 위해 사용하는 orthogonal 뷰 변환 행렬을 설정합니다.
	D3DXVECTOR3 eye(0, 0, -50.0f), at(0, 0, 0), up(0, 1.0f, 0);
	D3DXMatrixOrthoLH(&GetG().g_orthoProjMat, (FLOAT)pBackBufferSurfaceDesc->Width, (FLOAT)pBackBufferSurfaceDesc->Height, 0.1f, 100.0f);
	D3DXMatrixLookAtLH(&GetG().g_fixedViewMat,	&eye, &at, &up);

	TCHAR scrSizeString[64];
	StringCchPrintf( scrSizeString, 64, _T( "- INFO: Window width: %d / height: %d\n" ), GetG().m_scrWidth, GetG().m_scrHeight );
	OutputDebugString( scrSizeString );

	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
	GetG().m_camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 1.0f, 1000.0f );
	
	/// - G::m_screenFlash를 초기화
	GetG().m_screenFlash.onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc );

	/// - 쉐이더 초기화
	ConfigureShaders( pd3dDevice, pBackBufferSurfaceDesc );

	/// - 파티클 시스템 초기화 (현재는 사용하지 않습니다.)
	//ConfigureParticleSystem( pd3dDevice );
	
	/// - 타일 격자를 그리기 위한 프리미티브 및 기본 도형(::g_bst)을 초기화합니다.
	ConfigureTileGridGeometry( pd3dDevice );
	ConfigureTestGeometry( pd3dDevice );
	
	/// - 전역 광원을 설정합니다.
	GetEpLight().setupLight( pd3dDevice );

	/// - 프로그램 전반에 쓰이는 폰트(LPD3DXFONT) 초기화
	V_RETURN( D3DXCreateFont( pd3dDevice, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulimche"), &g_pFont) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 26, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T( "Arial Black"), &g_unitNameFont ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Gulim"), &g_dlgContentFont) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("msgothic"), &g_dlgNameFont) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 26, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T( "Palatino Linotype"), &g_d3dxFont ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 32, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T( "Palatino Linotype"), &g_d3dxFontBig ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &g_fontBattle2 ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 18, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Rockwell Extra Bold"), &g_fontBattle ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &g_fontSkill ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &g_fontSkillDescription ) );
	V_RETURN( D3DXCreateFont( pd3dDevice, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &g_fontStat ) );


	/// - 본 함수에서의 기본 초기화는 끝났습니다. 하부 구성원에게 OnCreateDevice 메시지를 전달합니다.
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
	GetTopStateManager().onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
	GetWorldManager().onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );
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

	g_fontBattle2->OnResetDevice();
	g_fontBattle->OnResetDevice();
	g_fontSkill->OnResetDevice();
	g_fontSkillDescription->OnResetDevice();
	g_fontStat->OnResetDevice();


	g_postRadialBlurShader->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc );
	g_postSepiaShader->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc );
	g_bombShader->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc );

	return hr;
}



/*!
	\brief 매 OnFrameMove() 시마다 RakNet 패킷 핸들링을 합니다.
	\details
	OnFrameMove() 마지막에 호출되는 함수입니다. 만일 클라이언트 피어 객체(::g_clientPeer)가
	존재한다면 RakNet이 초기화되었다는 뜻이므로 여기서 지난 프레임부터 현재프레임 사이에
	발생한 수신 패킷을 처리합니다.
*/
void OnFrameMoveNetworkProcess()
{
	if ( g_clientPeer )
	{
		Packet* p;
		for (p=g_clientPeer->Receive(); p; g_clientPeer->DeallocatePacket(p), p=g_clientPeer->Receive())
		{
			switch (p->data[0])
			{
			case ID_DISCONNECTION_NOTIFICATION:
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_ALREADY_CONNECTED:
				printf("ID_ALREADY_CONNECTED\n");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;
			case ID_PONG:
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("ID_NEW_INCOMING_CONNECTION\n");
				break;
			case ID_RPC_REMOTE_ERROR:
				{
					// Recipient system returned an error
					switch (p->data[1])
					{
					case RakNet::RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE:
						printf("RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE\n");
						break;
					case RakNet::RPC_ERROR_OBJECT_DOES_NOT_EXIST:
						printf("RPC_ERROR_OBJECT_DOES_NOT_EXIST\n");
						break;
					case RakNet::RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE:
						printf("RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE\n");
						break;
					case RakNet::RPC_ERROR_FUNCTION_NOT_REGISTERED:
						printf("RPC_ERROR_FUNCTION_NOT_REGISTERED\n");
						break;
					case RakNet::RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED:
						printf("RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED\n");
						break;
					case RakNet::RPC_ERROR_CALLING_CPP_AS_C:
						printf("RPC_ERROR_CALLING_CPP_AS_C\n");
						break;
					case RakNet::RPC_ERROR_CALLING_C_AS_CPP:
						printf("RPC_ERROR_CALLING_C_AS_CPP\n");
						break;
					}
					printf("Function: %s", p->data+2);
				}
			}
		}
	}
}




/*!
	\brief 매 프레임마다 렌더링 이외의 게임 로직을 계산합니다.
	\param fTime 프로그램이 시작된 후 현재까지 흐른 시간을 초 단위로 나타냅니다.
	\param fElapsedTime 지난 프레임부터 현재 프레임까지 걸린 시간을 초 단위로 나타냅니다.
	\param pUserContext 사용하지 않는 매개변수
	\warning 이 함수와 독립적으로 호출되는(즉, 이 함수로부터 호출되지 않는) 프레임 무브 관련 함수는
	구조적으로 잘못된 것입니다.
	\details
	DXUT에서 정의된 콜백 함수입니다.

	이 함수는 프레임이 렌더링 렌더링 될때마다 그 직전에 호출됩니다.
	화면에 그리는 작업 이외의 대부분 작업이 여기서 일어납니다. 카메라의 위치를 바꾼다거나,
	적군이 근처에 있어서 플레이어가 전투에 돌입해야하는지 판단한다거나 하는 일을 합니다.
	Child node의 OnFrameMove 함수가 재귀적으로 호출되는 곳도 여기입니다.
	함수명과 파라미터에서 알 수 있듯이 이것은 D3D9/10 디바이스와는 완전히 독립적으로 움직입니다.
	즉, 본 함수에서 D3D9 디바이스와 관련된 어떠한 변경이나 작업도 일어나서는 안됩니다.

	참고로 처음 DXUT 소스 코드에는 아래와 같은 커맨트가 있었습니다.

	Handle updates to the scene.  This is called regardless of which D3D API is used

	이후는 본 함수에서 일어나는 일 중 중요한 것을 나열한 것입니다.
*/
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;

	UNREFERENCED_PARAMETER( hr );

	/// - 만일 WorldManager에 변경해야 할 다음 World가 설정되어 있다면 변경합니다.
	GetWorldManager().changeToNextWorldIfExist();

	/// - 만일 TopStateManager에 변경해야 할 다음 스테이트가 설정되어 있다면 변경합니다.
	GetTopStateManager().transit( fTime );

	/// - 현재 TopState의 프레임 무브 함수를 호출합니다.
	if (GetTopStateManager().getCurState())
		GetTopStateManager().getCurState()->frameMove(fTime, fElapsedTime);

	/// - G::m_camera의 프레임 무브 함수를 호출합니다.
	GetG().m_camera.frameMove( fElapsedTime );

	/// - EpLight의 프레임 무브 함수를 호출합니다.
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
	
	/// - G::m_screenFlash의 프레임 무브 함수를 호출합니다.
	GetG().m_screenFlash.frameMove( fTime, fElapsedTime );

	/// - RakNet 패킷을 처리하기 위해 OnFrameMoveNetworkProcess() 함수를 호출합니다.
	OnFrameMoveNetworkProcess();
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
		g_bst[BST_TEAPOT]->DrawSubset( 0 );
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

HRESULT renderTileGrid( IDirect3DDevice9* pd3dDevice )
{
	HRESULT hr = S_OK;

	pd3dDevice->SetVertexShader( 0 );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pd3dDevice->SetTexture( 0, 0 );

	V( pd3dDevice->SetTransform( D3DTS_VIEW, GetG().m_camera.GetViewMatrix() ) );
	V( pd3dDevice->SetTransform( D3DTS_PROJECTION, GetG().m_camera.GetProjMatrix() ) );
	V( pd3dDevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE ) );
	V( pd3dDevice->SetStreamSource( 0, g_lineElement, 0, 16 ) );

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
		V( pd3dDevice->SetTransform( D3DTS_WORLD, &mWorld ) );
		V( pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 ) );
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
		V( pd3dDevice->SetTransform( D3DTS_WORLD, &mWorld ) );
		D3DPERF_BeginEvent( 0, L"Boundary Line Drawing" );
		V( pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 ) );
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



/*!
	\brief D3D9 디바이스를 이용해 화면에 렌더합니다.
	\param pd3dDevice D3D9 디바이스
	\param fTime 프로그램이 시작된 후 현재까지 흐른 시간을 초 단위로 나타냅니다.
	\param fElapsedTime 지난 프레임부터 현재 프레임까지 걸린 시간을 초 단위로 나타냅니다.
	\param pUserContext 사용하지 않는 매개변수
	\details
	DXUT에서 정의된 콜백 함수입니다.
	모든 게임 루프의 최상위에 위치하는 렌더링 루틴입니다. 매 프레임이 그려질 때 본 함수가
	호출됩니다. Child node의 OnFrameRender 함수가 재귀적으로 호출되는 곳도 여기입니다.
	\warning 이 함수와 독립적으로 호출되는(즉, 이 함수로부터 호출되지 않는) 렌더 관련 함수는
	구조적으로 잘못된 것입니다.
*/
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
			renderTileGrid( pd3dDevice );

		State* curState = GetTopStateManager().getCurState();

		curState->frameRender( pd3dDevice, fTime, fElapsedTime );

		renderFixedElements( fTime, fElapsedTime );

		g_spriteManager->frameRender();
		
		GetEpLight().frameRender( pd3dDevice );

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

#define SAFE_ONLOSTDEVICE(x) if (x) (x)->OnLostDevice();

	SAFE_ONLOSTDEVICE( g_pFont );
	SAFE_ONLOSTDEVICE( g_unitNameFont );
	SAFE_ONLOSTDEVICE( g_dlgNameFont );
	SAFE_ONLOSTDEVICE( g_dlgContentFont );
	SAFE_ONLOSTDEVICE( g_fontBattle2 );
	SAFE_ONLOSTDEVICE( g_fontBattle );
	SAFE_ONLOSTDEVICE( g_fontSkill );
	SAFE_ONLOSTDEVICE( g_fontSkillDescription );
	SAFE_ONLOSTDEVICE( g_fontStat );



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

	if ( g_postRadialBlurShader ) g_postRadialBlurShader->onLostDevice();
	if ( g_postSepiaShader ) g_postSepiaShader->onLostDevice();
	if ( g_bombShader ) g_bombShader->onLostDevice();

	GetG().m_screenFlash.onLostDevice();

	// GetG() related
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
	SAFE_RELEASE( g_bst[BST_TEAPOT] );
	SAFE_RELEASE( g_bst[BST_CUBE] );
	SAFE_RELEASE( g_bst[BST_PLANE] );
	SAFE_RELEASE( g_bst[BST_SPHERE] );

	// Tile grid geometry
	SAFE_RELEASE( g_lineElement );

	// Shaders
	EP_SAFE_RELEASE( g_bombShader );
	EP_SAFE_RELEASE( g_postSepiaShader );
	EP_SAFE_RELEASE( g_postRadialBlurShader );


	// GetG() related
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


	SAFE_RELEASE( g_fontBattle2 );
	SAFE_RELEASE( g_fontBattle );
	SAFE_RELEASE( g_fontSkill );
	SAFE_RELEASE( g_fontSkillDescription );
	SAFE_RELEASE( g_fontStat );


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

/*!
	\brief 실행 파일이 있는 곳으로 작업 디렉토리를 변경합니다.
	\details
	개발 중에는 MSVC에서 프로그램을 실행시키는 경우가 많은데, 작업 디렉토리가
	바뀌기가 쉬워서 경우에 따라 경로 설정을 다르게 해야하는 번거로움이 있습니다.
	본 함수가 현재 실행 파일이 있는 디렉토리를 작업 디렉토리(working directory)로
	만들어서 다른 모든 경로가 작업 디렉토리를 기준으로 상대 경로로 나타낼 수 있도록
	도와줍니다.
*/
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


/*!
	@brief 모든 전역변수를 초기화하고 렌더 루프로 돌입합니다.
	@details
	본 함수는 main()이거나 wWinMain()일 수 있습니다. EP 디버그 빌드에서는 윈도우 콘솔창에
	디버그 정보를 나타내기 위해 main()을 사용하고, EP 릴리즈 빌드에서는 콘솔창이 필요없으므로
	wWinMain()을 사용하도록 전처리계가 설정되어 있습니다.\n
	본 함수에서는 D3D Device(LPDIRECT3DDEVICE9 혹은IDirect3DDevice9*)과 무관한 부분을 초기화합니다.
	그리고 DXUT가 설정한 각종 콜백 함수를 등록하게 됩니다. 여기에는 OnD3D9CreateDevice(), OnD3D9ResetDevice(),
	OnD3D9FrameRender(), OnFrmaeMove(), OnD3D9LostDevice(), OnD3D9DestroyDevice() 등이 포함됩니다.
	콜백 함수 이름에서 알 수 있듯이 OnFrameMove는 D3D9/D3D10을 구분하지 않는 루틴입니다. 이는 다시 말해
	OnFrameMove에서는 D3D Device와 연관된 코드를 작성하지 않아야 함을 의미합니다.
*/
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

	ZeroMemory( g_bst, sizeof(LPD3DXMESH) * 4 );

	// TODO: Perform any application-level initialization here
	/*!
	애플리케이션 레벨의 초기화는 이 부분에서 일어납니다.
	D3D device를 얻기 전이므로 이와 연관된 초기화를 수행해서는 안됩니다.
	*/

	/**
	- Working Directory 설정\n
	현재 프로그램 실행파일이 있는 위치로 working directory를 변경합니다.
	*/
	SetCurrentWorkingDirectory();
	g_debugBuffer.resize( 2048 );
	
	/*!
	- ScriptManager 초기화\n
	Tcl 스크립트 언어를 해석하고 바인딩해주는 ScriptManager를 할당하고 초기화합니다.
	ScriptManager가 생성되어있지 않은 경우에만 생성해줍니다. 생성 여부를 체크하는 이유는
	이 ScriptManager가 다른 스레드에서 먼저 생성되는 경우도 있기 때문입니다.
	이후 게임 전반에 사용되는 스크립트 파일을 등록하게 됩니다.\n
	*/
	CreateScriptManagerIfNotExist();
	GetScriptManager().executeFile( "Script/EpInitScript.tcl" );
	GetScriptManager().executeFile( "Script/EpDialog1.tcl" );
	GetScriptManager().executeFile( "Script/EpSkill.tcl" );
	GetScriptManager().execute( "EpInitApp" );


	/*!
	- SpriteManager 초기화\n
	2D 그래픽을 단순히 찍어야 하는 경우(UI 등)에 사용하는 스프라이트를 관리해주는
	SpriteManager를 할당하고 초기화합니다. 반드시 SpriteManager는 이곳에서 초기화되어야 합니다.
	초기화 단계에서 미리 등록되는 스프라이트는 없으며, 이후에 명시적으로 등록한 후 사용해야 합니다.\n
	*/
	assert( g_spriteManager == 0 );
	g_spriteManager = new SpriteManager();


	/*! - TopStateManager (tsm) 초기화\n
	게임의 최상위 스테이트를 관리해주는 tsm을 초기화합니다. 최상위 스테이트에는 게임 실행시 처음으로 설정되는
	IntroState, 게임 플레이 중을 나타내는 PlayState, 제작자 목록을 보여주는 CreditState등이 있습니다.
	tsm의 초기화는 반드시 여기서, 그리고 프로그램 실행 주기에 단 한번 일어나야 합니다.\n
	*/
	assert( g_tsm == 0 );
	g_tsm = new TopStateManager();
	g_tsm->init();

	/** - WorldStateManager (wsm) 초기화\n
	TopStateManager 중 가장 큰 비중을 차지하는 PlayState는 또 몇 개의 하위 스테이트로 나누어져 있습니다.
	이러한 PlayState의 하위 스테이트를 관리해주는 클래스가 바로 wsm입니다.
	wsm의 초기화는 반드시 여기서, 그리고 프로그램 실행 주기에 단 한번 일어나야 합니다.\n
	*/
	assert( g_wsm == 0 );
	g_wsm = new WorldStateManager();
	g_wsm->init();
	
	/** - WorldManager (wm) 초기화\n
	EP의 구조상 레벨(맵) 개념은 World란 단어로 정의됩니다. 플레이어는 몇 개의 World를
	왔다갔다 할 수 있습니다. 이 공간은 비연속적일 수 있습니다. 이러한 World 객체를 관리해주는
	클래스가 바로 WorldManager입니다. WorldStateManager (wsm)과 혼동하지 않도록 주의하십시오.
	wm의 초기화는 반드시 여기서, 그리고 프로그램 실행 주기에 단 한번 일어나야 합니다.\n
	*/
	assert( g_wm == 0 );
	g_wm = new WorldManager();

	/** - 모든 World 스크립트 파일을 읽어오기\n
	각 World는 하나의 World 스크립트 파일에 의해 정의됩니다. 이전에 초기화된 ScriptManager를 이용해
	EpWorldList 스크립트 변수를 읽어들여 어떤 World가 정의되어있는지 파악한 후, 각 World 스크립트 파일을
	순차적으로 로드합니다. 이때, 각 World는 하나의 modelFilePath 스크립트 변수를 가지고 있는데,
	이것이 화면에 렌더링될 레벨의 ARN 모델 파일이 됩니다. 물론 지금은 D3D9 디바이스가 초기화되기 전이므로
	ARN 파일 이름만 로드해올 뿐 다른 일은 하지 않습니다.
	*/
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

	/** - 전역 카메라(G::m_camera) 초기화\n
	<미작성된 부분>
	*/
	GetG().m_camera.SetAttachCameraToModel( true );
	GetG().m_camera.SetEnablePositionMovement( true );

	/** - 전역 광원(::g_epLight) 초기화\n
	<미작성된 부분>
	*/
	assert( g_epLight == 0 );
	g_epLight = new EpLight();

	/** - 콘솔 스레드 초기화\n
	초기의 EP에서는 디버그 목적으로 콘솔 스레드가 따로 구성되어 있었습니다.
	그렇지만 시간이 지나면서 동기화 문제 및 메모리 누수 문제가 생겨 결국에는 콘솔 스레드를
	생성했다가 바로 해제하고 있는 상태입니다.
	*/
	g_scriptBindingFinishedEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	ResetEvent( g_scriptBindingFinishedEvent );
	uintptr_t t = _beginthread( EpConsoleThreadMain, 0, 0 );
	WaitForSingleObject( g_scriptBindingFinishedEvent, INFINITE );


	/** - 스크립트 측 초기화 프로시저 실행\n
	EpInitScript.tcl 파일에 정의된 EpOnCreateDevice 프로시저가 실행됩니다.
	현재 OnCreateDevice 상태는 아니지만 소스 코드가 어떻게 계속 바뀌다보니 이렇게 이상한
	이름이 된 상태입니다. 차후 정정할 예정입니다. :(
	*/
	GetScriptManager().execute( "EpOnCreateDevice" );


	/** - SkillManager 초기화\n
	Skill은 EP에서 정의한 '전투 중 가능한 행동'입니다. 이러한 행동은 매우 다양해질 수 있으므로
	이를 관리할 수 있는 SkillManager를 만들었습니다. SkillManager는 WorldManager와 마찬가지로
	스크립트 측에서 정의된 스킬 목록을 가져와서 초기화하게 됩니다. 이는 EpSkill.tcl에 정의된
	EpSkillList라는 스크립트 변수에서 확인할 수 있습니다.
	*/
	SkillManager* g_skillManager = new SkillManager();
	ConstCharList skillList;
	GetScriptManager().readCharPtrList( "EpSkillList", skillList );
	it = skillList.begin();
	for ( ; it != skillList.end(); ++it )
	{
		const char* skillName = *it;
		GetSKillManager().registerSkill( Skill::createSkillByScript( skillName ) );
	}

	/** - 초기 World를 설정\n
	기본적으로 IntroState 이후 PlayState에 진입하게 되는데, PlayState에서는 여러 개의
	로드된 World 중에 하나를 선택해야 합니다. 이러한 초기 World를 여기서 설정해줍니다.
	어떤 World가 초기 월드가 되는지는 EpInitScript.tcl 파일의 EpStartWorldName 스크립트	변수에
	정의되어 있습니다.
	*/
	const char* startWorldName = GetScriptManager().readString( "EpStartWorldName" );
	GetWorldManager().setNextWorld( startWorldName );
	//GetWorldManager().changeToNextWorldIfExist();

	/** - 창모드 혹은 전체화면 모드 설정\n
	*/
	const char* windowMode = GetScriptManager().readString( "EpWindowMode" );
	bool bWindowMode = (*windowMode)=='1'?true:false;


	/** - RakNet 초기화 및 서버 접속 시도\n
	RakNet 라이브러리는 멀티플레이어 게임 엔진입니다. RakNet을 적용한지 얼마 안되었으므로
	이 부분은 자주, 그리고 급격히 변경될 수 있습니다.
	*/
	const char* networkMode = GetScriptManager().readString( "NetworkMode" );
	if ( *networkMode == '1' )
		ConnectToServer();

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

	if ( g_clientPeer && g_clientPeer->IsConnected( g_clientPeer->GetSystemAddressFromIndex( 0 ) ) )
	{
		Unit* hero = GetWorldManager().getCurWorld()->getHero();

		GetWorldManager().getCurWorld()->detachUnit( hero );		// Detach from world since Hero will be deallocated by ReplicaManager.
		GetWorldManager().getCurWorld()->detachAllUnitBaseType();	// Detach from world since UnitBase typed instances will be deallocated by ReplicaManager.

		DisconnectFromServer();
	}
	
	if ( g_clientPeer )
	{
		// Be nice and let the server know we quit.
		g_clientPeer->Shutdown( 300 );

		// We're done with the network
		RakNetworkFactory::DestroyRakPeerInterface( g_clientPeer );
	}
	SAFE_DELETE( g_replicaManager );

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
	delete g_skillManager;
	delete g_epLight;
	
	Tcl_Finalize();

	// This should be done at the last stage of termination since
	// many replica objects have RakNet::RakString.
	RakNet::RakString::FreeMemory();

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
	UINT i;
	for ( i = 0; i < BST_COUNT; ++i )
	{
		assert( g_bst[i] == 0 );
	}
	//enum BasicShapeType { BST_UNKNOWN, BST_TEAPOT, BST_SPHERE, BST_CUBE, BST_PLANE, BST_COUNT };
	D3DXCreateTeapot( pd3dDevice, &g_bst[BST_TEAPOT], 0 );
	D3DXCreateSphere( pd3dDevice, 1.0f, 8, 8, &g_bst[BST_SPHERE], 0 );
	D3DXCreateBox( pd3dDevice, 1.0f, 1.0f, 1.0f, &g_bst[BST_CUBE], 0 );
	D3DXCreatePolygon( pd3dDevice, 1.0f, 4, &g_bst[BST_PLANE], 0 );
}


// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
		return (unsigned char) p->data[0];
}

void EpRpcDoScript( RakNet::RakString command )
{
	GetScriptManager().execute( command );
}

void ConnectToServer()
{
	//g_rpc3Inst = new RakNet::RPC3;
	g_replicaManager = new RakNet::ReplicaManager2;

	UnitBase::mySoldier=0;
	EpUser::myUser=0;

	g_networkIDManager.SetIsNetworkIDAuthority( false );
	g_rpc3Inst.SetNetworkIDManager( &g_networkIDManager );

	RPC3_REGISTER_FUNCTION( &g_rpc3Inst, PrintHelloWorld );
	RPC3_REGISTER_FUNCTION( &g_rpc3Inst, EpRpcDoScript );
	RPC3_REGISTER_FUNCTION( &g_rpc3Inst, &Unit::setTilePosRpc );

	
	g_clientPeer = RakNetworkFactory::GetRakPeerInterface();
	g_clientPeer->SetNetworkIDManager( &g_networkIDManager );

	// Holds packets
	Packet* p;

	// GetPacketIdentifier returns this
	unsigned char packetIdentifier;

	// Just so we can remember where the packet came from
	bool isServer;

	// Record the first client that connects to us so we can pass it to the ping function
	SystemAddress clientID = UNASSIGNED_SYSTEM_ADDRESS;

	// Holds user data
	char ip[30], serverPort[30], clientPort[30];

	// A client
	isServer = false;

	printf( "This is a sample implementation of a text based chat client.\n" );
	printf( "Connect to the project 'Chat Example Server'.\n" );
	printf( "Difficulty: Beginner\n\n" );

	// Get our input
	puts( "Enter the client port to listen on" );
	/*gets( clientPort );
	if ( clientPort[0] == 0 )
		strcpy( clientPort, "0" );*/
	strcpy_s( clientPort, 30, "0" );
	

	puts( "Enter IP to connect to" );
	//gets( ip );
	g_clientPeer->AllowConnectionResponseIPMigration(false);
	//if (ip[0]==0)
		strcpy_s(ip, 30, "127.0.0.1");


	puts("Enter the port to connect to");
	//gets(serverPort);
	//if (serverPort[0]==0)
		strcpy_s(serverPort, 30, "10000");

	// Connecting the client is very simple.  0 means we don't care about
	// a connectionValidationInteger, and false for low priority threads
	SocketDescriptor socketDescriptor((unsigned short)atoi(clientPort),0);
	g_clientPeer->Startup(1,30,&socketDescriptor, 1);
	g_clientPeer->AttachPlugin( g_replicaManager );
	// Just test this
	g_replicaManager->SetAutoAddNewConnections( false );
	// Register our custom connection factory
	g_replicaManager->SetConnectionFactory( &g_connectionFactory );
	g_clientPeer->SetOccasionalPing(true);
	bool b = g_clientPeer->Connect(ip, (unsigned short)atoi(serverPort), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"));	

	g_clientPeer->AttachPlugin( &g_rpc3Inst );

	RakNet::StringTable::Instance()->AddString( "UnitBase", false );
	RakNet::StringTable::Instance()->AddString( "EpUser", false );


	if (b)
	{
		puts("Attempting connection");

		//puts("'quit' to quit. 'stat' to show stats. 'ping' to ping. 'disconnect' to disconnect. Type to talk.");

		bool doLoop = true;

		while ( doLoop )
		{
			Sleep(30);

			p = g_clientPeer->Receive();


			if (p==0)
				continue; // Didn't get any packets

			// We got a packet, get the identifier with our handy function
			packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				doLoop = false;
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				printf("ID_ALREADY_CONNECTED\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_CONNECTION_LOST\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				printf("We are banned from this server.\n");
				break;			
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				doLoop = false;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;
			case ID_MODIFIED_PACKET:
				// Cheater!
				printf("ID_MODIFIED_PACKET\n");
				break;

			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
				g_replicaManager->AddNewConnection( p->systemAddress );

				doLoop = false;
				break;
			default:
				// It's a client, so just show the message
				printf( "%s\n", p->data );
				break;
			}


			// We're done with the packet
			g_clientPeer->DeallocatePacket( p );

		}

	}
	else
	{
		puts( "Bad connection attempt.  Terminating." );
		DebugBreak();
	}
}

void DisconnectFromServer()
{
	if ( g_clientPeer == 0 )
		return;

	Packet* p;
	unsigned char packetIdentifier;

	if ( g_clientPeer->IsConnected( g_clientPeer->GetSystemAddressFromIndex( 0 ) ) )
	{
		g_clientPeer->CloseConnection( g_clientPeer->GetSystemAddressFromIndex( 0 ), true, 0 );
		printf( "Disconnecting.\n" );

		bool doLoop = true;
		while ( doLoop )
		{
			Sleep(30);

			p = g_clientPeer->Receive();


			if ( p == 0 )
				continue; // Didn't get any packets

			// We got a packet, get the identifier with our handy function
			packetIdentifier = GetPacketIdentifier( p );

			// Check if this is a network message packet
			switch ( packetIdentifier )
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				EpUser::DeleteAllUsers();
				doLoop = false;
				break;
			}

			// We're done with the packet
			g_clientPeer->DeallocatePacket( p );
		}
	}

	

	
	//SAFE_DELETE( g_rpc3Inst );
	
}
