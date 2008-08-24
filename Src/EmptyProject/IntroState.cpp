#include "EmptyProjectPCH.h"
#include "IntroState.h"
#include "Picture.h"
#include "TopStateManager.h"
#include "EpLIght.h"
#include "SpriteManager.h"

//The Great Adventure of PoolG"
//~ PoolG's Strikes Back ~"
//
//It is an age of darkness! Evil lord Choi 
//revealed his unholy desire. Republic's 
//Finance and Environment is already in his 
//hand. No freedom of speech, and no freedom 
//of gaming. People suffer from 20 hours 
//coding labor. There are heroes on both sides.
//
//After 'Big Cleaning', PoolG lost home, food,
//dental cream, everything. For revenge, PoolG 
//trained coding very hard. He accepted the 
//dark side of coding spirit. He gained power 
//of infinite loop, memory leaking, and so on.
//Now PoolG's difficult journey is just 
//beginning.

IntroState::IntroState(void)
{
	UINT i;
	for (i = 0; i < NUM_OF_LINES; ++i)
	{
		// Avoid non-invertible world-view matrix
		D3DXMatrixIdentity(&m_matObjs[i]);
		m_pTextMeshes[i] = 0;
	}

	m_fFadeTimer = 0.0f;
	m_pLogo = 0;
	m_pLogoDrawRequest = 0;
	m_pGalaxy = 0;
	m_pGalaxyDrawRequest = 0;
}

IntroState::~IntroState(void)
{
}

HRESULT IntroState::enter()
{
	LPDIRECT3DDEVICE9 pd3dDevice = GetG().m_dev;

	// Initialize arrays as NULL
	for( int i = 0; i < NUM_OF_LINES; i++ )
	{
		m_pStrs[i] = NULL;
		m_pTextMeshes[i] = NULL;
	}

	// Initialize member variables
	m_pStrFont = L"Georgia";
	m_bBold = true;
	m_bItalic = false;
	m_dwFontSize = 10;

	m_logoVisible = true;
	m_backgroundVisible = false;

	m_velocity = 1.2f;

	m_pStrs[0] = L"The Great Adventure of PoolG";
	m_pStrs[1] = L"~ PoolG's Strikes Back ~";
	m_pStrs[3] = L"It is an age of darkness! Evil lord Choi ";
	m_pStrs[4] = L"revealed his unholy desire. Republic's ";
	m_pStrs[5] = L"Finance and Environment is already in his ";
	m_pStrs[6] = L"hand. No freedom of speech, and no freedom ";
	m_pStrs[7] = L"of gaming. People suffer from 20 hours ";
	m_pStrs[8] = L"coding labor. There are heroes on both sides.";
	m_pStrs[10] = L"After 'Big Cleaning', PoolG lost home, food,";
	m_pStrs[11] = L"dental cream, everything. For revenge, PoolG ";
	m_pStrs[12] = L"trained coding very hard. He accepted the ";
	m_pStrs[13] = L"dark side of coding spirit. He gained power ";
	m_pStrs[14] = L"of infinite loop, memory leaking, and so on.";	
	m_pStrs[15] = L"Now PoolG's difficult journey is just";
	m_pStrs[16] = L"beginning!";

	// Create text meshes
	createTextMeshes(GetG().m_dev);

	// Setup light and camera
	setupLight();
	setupCamera();

	m_sound.init(); // 사운드 초기화

	//pd3dDevice->SetTransform( D3DTS_WORLD, &DX_CONSTS::D3DXMAT_IDENTITY );


	m_pLogo = GetSpriteManager().registerSprite( "PoolCLogo", "Images/PoolC_Square.png" );
	m_pLogo->registerRect( "PoolCLogo", 0, 0, 248, 129 );

	D3DXVECTOR3 logoPos( GetG().m_scrWidth / 2.0f - 124.0f, GetG().m_scrHeight / 2.0f - 64.5f, 0.0f );
	D3DXCOLOR logoColor( 1.0f, 1.0f, 1.0f, 0.0f );
	m_pLogoDrawRequest = m_pLogo->drawRequest( "PoolCLogo", 0, &logoPos, logoColor );


	m_pGalaxy = GetSpriteManager().registerSprite( "Galaxy", "Images/the_Whirlpool_Galaxy_M51.jpg" );
	m_pGalaxy->registerRect( "Galaxy", 0, 0, 1280, 1280 );

	D3DXVECTOR3 galaxyPos( 0.0f, 0.0f, 3.0f );
	D3DXCOLOR galaxyColor( 1.0f, 1.0f, 1.0f, 0.0f );
	m_pGalaxyDrawRequest = m_pGalaxy->drawRequest( "Galaxy", 0, &galaxyPos, galaxyColor );


	m_pPrologue = GetSpriteManager().registerSprite( "Prologue", "Images/PoolG_Prologue.png" );
	m_pPrologue->registerRect( "Prologue", 0, 0, 512, 1024 );
	
	D3DXVECTOR3 prologuePos( 0.0f, 0.0f, 0.0f );
	D3DXCOLOR prologueColor( 1.0f, 1.0f, 1.0f, 1.0f );
	m_pPrologueDrawRequest = m_pPrologue->drawRequestXformable( "Prologue" );



	return S_OK;
}

HRESULT IntroState::leave()
{
	m_startTime = -1.0f;

	return S_OK;
}

HRESULT IntroState::frameMove( double fTime, float fElapsedTime )
{
	if ( fElapsedTime <= 0.1f )
		m_fFadeTimer += fElapsedTime;

	if ( 0.0f <= m_fFadeTimer && m_fFadeTimer <= 6.0f )
	{
		float ratio = (float)m_fFadeTimer / 6.0f;
		m_pLogoDrawRequest->color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, sin( D3DXToRadian( ratio * 180.0f ) ) );
	}
	else
	{
		if ( m_pLogoDrawRequest != 0 )
			m_pLogo->removeDrawRequest( m_pLogoDrawRequest );
	}

	/*else if ( 6.0f <= m_fFadeTimer && m_fFadeTimer < 12.0f )
	{
		float ratio = (float)(m_fFadeTimer - 6.0f) / 6.0f;
		m_pGalaxyDrawRequest->color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, sin( D3DXToRadian( ratio * 90.0f ) ) );
	}
	else if ( 40.0f <= m_fFadeTimer && m_fFadeTimer < 46.0f )
	{
		float ratio = (float)(m_fFadeTimer - 40.0f) / 6.0f;
		m_pGalaxyDrawRequest->color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, sin( D3DXToRadian( ratio * 90.0f + 90.0f ) ) );
	}*/

	if ( 6.0f <= m_fFadeTimer && m_fFadeTimer < 46.0f )
		m_sound.Opening(); // 오프닝 사운드 시작

	if ( 0.0f <= m_fFadeTimer && m_fFadeTimer < 56.0f )
	{
		//// Add some translational values to the matrices
		//for (int i = 0; i < NUM_OF_LINES; i++)
		//{
		//	D3DXMatrixTranslation( 
		//		&m_matObjs[i], 
		//		-12.0f, 
		//		-20 - ( float) i * 2.0f + ( float ) (m_fFadeTimer - 6.0f) * m_velocity,
		//		0.0f );
		//}

		D3DXMATRIX prologueTrans;
		D3DXMATRIX prologueScale;
		D3DXMatrixScaling( &prologueScale, 0.03f, -0.03f, 0.03f );
		D3DXMatrixTranslation( 
			&prologueTrans, 0.0f, 
			-35.0f + ( float ) (m_fFadeTimer - 6.0f) * m_velocity,
			0.0f );
		m_pPrologueDrawRequest->xform = prologueScale * prologueTrans;

	}
	else if ( 56.0f <= m_fFadeTimer )
	{
		if ( m_pPrologueDrawRequest != 0 )
			m_pPrologue->removeDrawRequest( m_pPrologueDrawRequest );

		TopStateManager::getSingleton().setNextState( GAME_TOP_STATE_PLAY );
	}

	return S_OK;
}

HRESULT IntroState::frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{
	/*D3DMATERIAL9 mtrl;*/

	//pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	// Apply fixed camera
	pd3dDevice->SetTransform(D3DTS_VIEW, &GetG().g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &GetG().g_orthoProjMat);
	
	// Setup view and projection xforms - ?
	pd3dDevice->SetTexture(0, 0);

	// Apply non-fixed camera
	pd3dDevice->SetTransform(D3DTS_VIEW, GetG().m_camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, GetG().m_camera.GetProjMatrix());

	/*if( m_pTextMeshes[0] != NULL )
	{
		ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
		
		D3DXCOLOR cv( 1.0f * m_fTextAlpha, 0.8f * m_fTextAlpha, 0.0f, m_fTextAlpha );
		mtrl.Diffuse  = cv;
		mtrl.Ambient = cv;
		mtrl.Ambient = cv;

		pd3dDevice->SetMaterial( &mtrl );

		for (int i = 0; i < NUM_OF_LINES; i++)
		{
			if ( m_pTextMeshes[i] == NULL )
				continue;

			pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObjs[i] );
			m_pTextMeshes[i]->DrawSubset( 0 );
		}
	}*/

	return S_OK;
}

HRESULT IntroState::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN)
	{
		if ( wParam == VK_ESCAPE )
		{
			if ( m_pLogoDrawRequest != 0 )
				m_pLogo->removeDrawRequest( m_pLogoDrawRequest );
			if ( m_pPrologueDrawRequest != 0 )
				m_pPrologue->removeDrawRequest( m_pPrologueDrawRequest );
			TopStateManager::getSingleton().setNextState( GAME_TOP_STATE_PLAY );
		}
	}

	return S_OK;
}

HRESULT IntroState::release()
{
	for( int i = 0; i < NUM_OF_LINES; i++ )
	{		
		SAFE_RELEASE( m_pTextMeshes[i] );
	}
	
	m_pBlack.release();

	if ( m_pGalaxy )
		m_pGalaxy->release();
	if ( m_pLogo ) 
		m_pLogo->release();
	if ( m_pLogoDrawRequest )
		m_pLogoDrawRequest->release();
	if ( m_pGalaxyDrawRequest )
		m_pGalaxyDrawRequest->release();

	return S_OK;
}


void IntroState::setupLight()
{
	GetEpLight().setBrightness( 1.0f );
	GetEpLight().setFadeDuration( 0.1f );
	GetEpLight().fadeInLight();
}

void IntroState::setupCamera()
{
	D3DXVECTOR3 vecEye( 0.0f, -30.0f, -20.0f );
	D3DXVECTOR3 vecAt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vecUp( 0.0f, 1.0f, 0.0f );
	GetG().m_camera.setViewParamsWithUp( &vecEye, &vecAt, &vecUp );
}

HRESULT IntroState::createTextMeshes( IDirect3DDevice9* pd3dDevice )
{
	for( int i = 0; i < NUM_OF_LINES; i++ )
	{
		if ( m_pStrs[i] == NULL )
		{
			continue;
		}

		if( FAILED( createD3DXTextMesh( pd3dDevice, m_pStrs[i], &(m_pTextMeshes[i]) ) ) )
			return E_FAIL;
	}

	return S_OK;
}

HRESULT IntroState::createD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPCWSTR pStr, LPD3DXMESH* ppTextMesh )
{
	HRESULT hr;
	LPD3DXMESH pMeshNew = NULL;
	HDC hdc = CreateCompatibleDC( NULL );
	if( hdc == NULL )
		return E_OUTOFMEMORY;
	INT nHeight = -MulDiv( m_dwFontSize, GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
	HFONT hFont;
	HFONT hFontOld;

	hFont = CreateFont( nHeight, 0, 0, 0, m_bBold ? FW_BOLD : FW_NORMAL, m_bItalic, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		m_pStrFont );

	hFontOld = ( HFONT )SelectObject( hdc, hFont );

	hr = D3DXCreateText( pd3dDevice, hdc, pStr,
		0.01f, 0.2f, &pMeshNew, NULL, NULL );

	SelectObject( hdc, hFontOld );
	DeleteObject( hFont );
	DeleteDC( hdc );

	if( SUCCEEDED( hr ) )
		*ppTextMesh = pMeshNew;

	return hr;
}