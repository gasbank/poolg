//-----------------------------------------------------------------------------
//           Name: dx9_particle_system.cpp
//         Author: Kevin Harris
//  Last Modified: 05/23/05
//    Description: Particle system sample using hardware accelerated point 
//                 sprites.
//
//   Control Keys: Up         - View moves forward
//                 Down       - View moves backward
//                 Left       - View strafes left
//                 Right      - View strafes Right
//                 Left Mouse - Perform looking
//                 Mouse      - Look about the scene
//                 F1         - Change active particle system
//                 F2         - Increase particle size
//                 F3         - Decrease particle size
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "resource.h"
#include "particlesystem.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND                    g_hWnd          = NULL;
LPDIRECT3D9             g_pD3D          = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice    = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
LPDIRECT3DTEXTURE9      g_pTexture      = NULL;

POINT  g_ptLastMousePosit;
POINT  g_ptCurrentMousePosit;
bool   g_bMousing = false;
float  g_fMoveSpeed = 25.0f;
float  g_fElpasedTime;
double g_dCurTime;
double g_dLastTime;

D3DXVECTOR3	g_vEye(0.0f, 3.0f, -15.0f); // Eye Position
D3DXVECTOR3	g_vLook(0.0f, 0.0f, 1.0f);  // Look Vector
D3DXVECTOR3	g_vUp(0.0f, 1.0f, 0.0f);    // Up Vector
D3DXVECTOR3	g_vRight(1.0f, 0.0f, 0.0f); // Right Vector

struct Vertex
{
    FLOAT x,  y,  z;
	FLOAT tu, tv;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
	};
};

Vertex g_floorVertices[] =
{
	//  x     y     z     tu    tv 
    { -5.0f, 0.0f,-5.0f,  0.0f, 0.0f,},
    { -5.0f, 0.0f, 5.0f,  0.0f, 1.0f,},
    {  5.0f, 0.0f,-5.0f,  1.0f, 0.0f,},
    {  5.0f, 0.0f, 5.0f,  1.0f, 1.0f,},
};

int g_nActiveSystem = 1;
CParticleSystem *g_pParticleSystems[6];

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void loadTexture(void);
void init(void);
void shutDown(void);
void render(void);
void getRealTimeUserInput(void);
void updateViewMatrix(void);
HRESULT initParticles(void);

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow )
{
	WNDCLASSEX winClass; 
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS", 
                             "Direct3D (DX9) - Particle System Using Point Sprites",
						     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					         0, 0, 640, 480, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	init();
	initParticles();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else
		{
			g_dCurTime     = timeGetTime();
			g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);
			g_dLastTime    = g_dCurTime;

		    render();
		}
	}

	shutDown();

    UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{
    switch( msg )
	{
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				case VK_F1:
				{
					++g_nActiveSystem;

					if( g_nActiveSystem >= 6 )
						g_nActiveSystem = 0;

					g_pParticleSystems[g_nActiveSystem]->RestartParticleSystem();
				}
				break;

				case VK_F2:
				{
					float fSize = g_pParticleSystems[g_nActiveSystem]->GetSize();

					fSize += 0.1f;

					if( fSize >= g_pParticleSystems[g_nActiveSystem]->GetMaxPointSize() )
						fSize = g_pParticleSystems[g_nActiveSystem]->GetMaxPointSize();

					g_pParticleSystems[g_nActiveSystem]->SetSize( fSize );
				}
				break;

				case VK_F3:
				{
					float fSize = g_pParticleSystems[g_nActiveSystem]->GetSize();

					fSize -= 0.1f;

					if( fSize <= 0.1f )
						fSize = 0.1f;

					g_pParticleSystems[g_nActiveSystem]->SetSize( fSize );
				}
				break;
			}
		}
        break;

        case WM_LBUTTONDOWN:
		{
			g_bMousing = true;
		}
		break;

		case WM_LBUTTONUP:
		{
			g_bMousing = false;
		}
		break;

		case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}
		
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name: getRealTimeUserInput()
// Desc: 
//-----------------------------------------------------------------------------
void getRealTimeUserInput( void )
{
	//
	// Get mouse input...
	//

	POINT mousePosit;
	GetCursorPos( &mousePosit );
	ScreenToClient( g_hWnd, &mousePosit );

    g_ptCurrentMousePosit.x = mousePosit.x;
    g_ptCurrentMousePosit.y = mousePosit.y;

	D3DXMATRIX matRotation;

    if( g_bMousing )
    {
		int nXDiff = (g_ptCurrentMousePosit.x - g_ptLastMousePosit.x);
        int nYDiff = (g_ptCurrentMousePosit.y - g_ptLastMousePosit.y);
        
        if( nYDiff != 0 )
		{
			D3DXMatrixRotationAxis( &matRotation, &g_vRight, D3DXToRadian((float)nYDiff / 3.0f));
			D3DXVec3TransformCoord( &g_vLook, &g_vLook, &matRotation );
			D3DXVec3TransformCoord( &g_vUp, &g_vUp, &matRotation );
		}

		if( nXDiff != 0 )
		{
			D3DXMatrixRotationAxis( &matRotation, &D3DXVECTOR3(0,1,0), D3DXToRadian((float)nXDiff / 3.0f) );
			D3DXVec3TransformCoord( &g_vLook, &g_vLook, &matRotation );
			D3DXVec3TransformCoord( &g_vUp, &g_vUp, &matRotation );
		}
    }

    g_ptLastMousePosit.x = g_ptCurrentMousePosit.x;
    g_ptLastMousePosit.y = g_ptCurrentMousePosit.y;

	//
	// Get keyboard input...
	//

	unsigned char keys[256];
	GetKeyboardState( keys );

    D3DXVECTOR3 tmpLook  = g_vLook;
	D3DXVECTOR3 tmpRight = g_vRight;

	// Up Arrow Key - View moves forward
	if( keys[VK_UP] & 0x80 )
		g_vEye -= tmpLook*-g_fMoveSpeed*g_fElpasedTime;

	// Down Arrow Key - View moves backward
	if( keys[VK_DOWN] & 0x80 )
		g_vEye += (tmpLook*-g_fMoveSpeed)*g_fElpasedTime;

	// Left Arrow Key - View side-steps or strafes to the left
	if( keys[VK_LEFT] & 0x80 )
		g_vEye -= (tmpRight*g_fMoveSpeed)*g_fElpasedTime;

	// Right Arrow Key - View side-steps or strafes to the right
	if( keys[VK_RIGHT] & 0x80 )
		g_vEye += (tmpRight*g_fMoveSpeed)*g_fElpasedTime;

	// Home Key - View elevates up
	if( keys[VK_HOME] & 0x80 )
		g_vEye.y += g_fMoveSpeed*g_fElpasedTime; 

	// End Key - View elevates down
	if( keys[VK_END] & 0x80 )
		g_vEye.y -= g_fMoveSpeed*g_fElpasedTime;
}

//-----------------------------------------------------------------------------
// Name: loadTexture()
// Desc: 
//-----------------------------------------------------------------------------
void loadTexture(void)	
{
    D3DXCreateTextureFromFile( g_pd3dDevice, "dx.bmp", &g_pTexture );

	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

    D3DDISPLAYMODE d3ddm;

    g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

    d3dpp.Windowed               = TRUE;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                          D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                          &d3dpp, &g_pd3dDevice );
	
	D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3(0.0f, 0.0f,-10.0f), 
		                          &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		                          &D3DXVECTOR3(0.0f, 1.0f, 0.0f ) );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 1.0f, 1.0f, 200.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );

	//
	// Create the floor geometry...
	//

    loadTexture();

	g_pd3dDevice->CreateVertexBuffer( 4*sizeof(Vertex), D3DUSAGE_WRITEONLY, 
		                              Vertex::FVF_Flags, D3DPOOL_DEFAULT, 
                                      &g_pVertexBuffer, NULL );

    void *pVertices;
    g_pVertexBuffer->Lock( 0, sizeof(g_floorVertices), (void**)&pVertices, 0 );

    memcpy( pVertices, g_floorVertices, sizeof(g_floorVertices) );
    g_pVertexBuffer->Unlock();
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )
{
	for( int i = 0; i < 6; ++i )
	{
		if( g_pParticleSystems[i] != NULL )
		{
			delete g_pParticleSystems[i];
			g_pParticleSystems[i] = NULL;
		}
	}

    if( g_pVertexBuffer != NULL )
        g_pVertexBuffer->Release(); 

	if( g_pTexture != NULL )
		g_pTexture->Release();

    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: initParticles()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT initParticles( void )
{
	//
    // Exploding burst
	//
    
    g_pParticleSystems[0] = new CParticleSystem();

    g_pParticleSystems[0]->SetTexture( "particle.bmp" );
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

    g_pParticleSystems[0]->Init( g_pd3dDevice );

	//
    // Wind blown fountain
	//

	g_pParticleSystems[1] = new CParticleSystem();

    g_pParticleSystems[1]->SetTexture( "particle.bmp" );
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

    g_pParticleSystems[1]->Init( g_pd3dDevice );

	//
    // Omni-directiional emission expanding into space with no air resistence
	//

    g_pParticleSystems[2] = new CParticleSystem();

    g_pParticleSystems[2]->SetTexture( "particle.bmp" );
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

    g_pParticleSystems[2]->Init( g_pd3dDevice );
 
	//
    // Fountain particles behave like paint spots when striking a plane as 
	// the wind blowing them towards us
	//

    g_pParticleSystems[3] = new CParticleSystem();

    g_pParticleSystems[3]->SetTexture( "particle.bmp" );
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

    g_pParticleSystems[3]->Init( g_pd3dDevice );

	//
    // Fountain using a single collision plane acting as a floor
	//

    g_pParticleSystems[4] = new CParticleSystem();

    g_pParticleSystems[4]->SetTexture( "particle.bmp" );
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

    g_pParticleSystems[4]->Init( g_pd3dDevice );

	//
    // Fountain boxed-in by 6 collision planes
	//

    g_pParticleSystems[5] = new CParticleSystem();

    g_pParticleSystems[5]->SetTexture( "particle.bmp" );
    g_pParticleSystems[5]->SetMaxParticles( 100 );
    g_pParticleSystems[5]->SetNumToRelease( 5 );
    g_pParticleSystems[5]->SetReleaseInterval( 0.05f );
    g_pParticleSystems[5]->SetLifeCycle( 5.0f );
    g_pParticleSystems[5]->SetSize( 1.5f );
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

    g_pParticleSystems[5]->Init( g_pd3dDevice );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name : updateViewMatrix()
// Desc : Builds a view matrix suitable for Direct3D.
//
// Here's what the final matrix should look like:
//
//  |   rx     ux     lx    0 |
//  |   ry     uy     ly    0 |
//  |   rz     uz     lz    0 |
//  | -(r.e) -(u.e) -(l.e)  1 |
//
// Where r = Right vector
//       u = Up vector
//       l = Look vector
//       e = Eye position in world space
//       . = Dot-product operation
//
//-----------------------------------------------------------------------------
void updateViewMatrix( void )
{
	D3DXMATRIX view;
	D3DXMatrixIdentity( &view );

	D3DXVec3Normalize( &g_vLook, &g_vLook );
	D3DXVec3Cross( &g_vRight, &g_vUp, &g_vLook );
	D3DXVec3Normalize( &g_vRight, &g_vRight );
	D3DXVec3Cross( &g_vUp, &g_vLook, &g_vRight );
	D3DXVec3Normalize( &g_vUp, &g_vUp );

	view._11 = g_vRight.x;
    view._12 = g_vUp.x;
    view._13 = g_vLook.x;
	view._14 = 0.0f;

	view._21 = g_vRight.y;
    view._22 = g_vUp.y;
    view._23 = g_vLook.y;
	view._24 = 0.0f;

	view._31 = g_vRight.z;
    view._32 = g_vUp.z;
    view._33 = g_vLook.z;
	view._34 = 0.0f;

	view._41 = -D3DXVec3Dot( &g_vEye, &g_vRight );
	view._42 = -D3DXVec3Dot( &g_vEye, &g_vUp );
	view._43 = -D3DXVec3Dot( &g_vEye, &g_vLook );
	view._44 =  1.0f;

	g_pd3dDevice->SetTransform( D3DTS_VIEW, &view ); 
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: 
//-----------------------------------------------------------------------------
void render( void )
{
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0, 1.0f), 1.0f, 0 );

	getRealTimeUserInput();
	updateViewMatrix();

	//
	// The particle system will need to know how much time has passed since 
	// the last time it was updated, so we'll need to keep track of how much   
	// time has elapsed since the last frame update...
	//

	g_pParticleSystems[g_nActiveSystem]->Update( (float)g_fElpasedTime );

    g_pd3dDevice->BeginScene();

	//
	// Render the floor...
	//

    g_pd3dDevice->SetTexture( 0, g_pTexture );

    g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(Vertex) );
    g_pd3dDevice->SetFVF( Vertex::FVF_Flags );
    g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

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

    g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

    g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	
	//
	// Render particle system
	//

    g_pd3dDevice->SetTexture( 0, g_pParticleSystems[g_nActiveSystem]->GetTextureObject() );
	g_pParticleSystems[g_nActiveSystem]->Render( g_pd3dDevice );

	//
    // Reset render states...
	//

    g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

    g_pd3dDevice->EndScene();
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}
