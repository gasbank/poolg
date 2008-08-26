#include "EmptyProjectPCH.h"
#include "IntroState.h"
#include "Picture.h"
#include "TopStateManager.h"
#include "EpLIght.h"
#include "SpriteManager.h"

IntroState::IntroState(void)
{
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

	m_logoVisible = true;
	m_backgroundVisible = false;

	m_velocity = 1.2f;

	// Setup light and camera
	setupLight();
	setupCamera();

	//m_sound.init(); // 사운드 초기화

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

	if ( 0.0f <= m_fFadeTimer && m_fFadeTimer < 46.0f )
	{	
		D3DXMATRIX prologueTrans;
		D3DXMATRIX prologueScale;
		D3DXMatrixScaling( &prologueScale, 0.03f, -0.03f, 0.03f );
		D3DXMatrixTranslation( 
			&prologueTrans, 0.0f, 
			-35.0f + ( float ) (m_fFadeTimer - 6.0f) * m_velocity,
			0.0f );
		m_pPrologueDrawRequest->xform = prologueScale * prologueTrans;

	}
	else if ( 46.0f <= m_fFadeTimer )
	{
		if ( m_pPrologueDrawRequest != 0 )
			m_pPrologue->removeDrawRequest( m_pPrologueDrawRequest );

		TopStateManager::getSingleton().setNextState( GAME_TOP_STATE_PLAY );
	}

	if ( 6.0f <= m_fFadeTimer && m_fFadeTimer < 46.0f )
		m_sound.Opening(); // 오프닝 사운드 시작

	return S_OK;
}

HRESULT IntroState::frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	
	// Setup view and projection xforms - ?
	pd3dDevice->SetTexture(0, 0);

	// Apply non-fixed camera
	pd3dDevice->SetTransform(D3DTS_VIEW, GetG().m_camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, GetG().m_camera.GetProjMatrix());

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