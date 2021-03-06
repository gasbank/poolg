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
	m_nextState = GAME_TOP_STATE_MAIN;
	m_prologueInitZPos =  -34.0f;



	m_pLogo = GetSpriteManager().registerSprite( "PoolCLogo", "Images/PoolC_Square.png" );
	m_pLogo->registerRect( "PoolCLogo", 0, 0, 248, 129 );

	ArnVec3 logoPos( GetG().m_scrWidth / 2.0f - 124.0f, GetG().m_scrHeight / 2.0f - 64.5f, 0.0f );
	D3DXCOLOR logoColor( 1.0f, 1.0f, 1.0f, 0.0f );
	m_pLogoDrawRequest = m_pLogo->drawRequest( "PoolCLogo", 0, &logoPos, logoColor );


	m_pGalaxy = GetSpriteManager().registerSprite( "Galaxy", "Images/the_Whirlpool_Galaxy_M51.jpg" );
	m_pGalaxy->registerRect( "Galaxy", 0, 0, 1280, 1280 );

	ArnVec3 galaxyPos( 0.0f, 0.0f, 3.0f );
	D3DXCOLOR galaxyColor( 1.0f, 1.0f, 1.0f, 0.0f );
	m_pGalaxyDrawRequest = m_pGalaxy->drawRequest( "Galaxy", 0, &galaxyPos, galaxyColor );


	m_pPrologue = GetSpriteManager().registerSprite( "Prologue", "Images/PoolG_Prologue.png" );
	m_pPrologue->registerRect( "Prologue", 0, 0, 512, 1024 );


	ArnVec3 prologuePos( 0.0f, 0.0f, 0.0f );
	D3DXCOLOR prologueColor( 1.0f, 1.0f, 1.0f, 1.0f );
	m_pPrologueDrawRequest = m_pPrologue->drawRequestXformable( "Prologue" );


}

IntroState::~IntroState(void)
{
}

HRESULT IntroState::enter( double dStartTime )
{
	m_logoVisible = true;
	m_backgroundVisible = false;

	m_velocity = 1.2f;

	// Setup light and camera
	setupLight();
	setupCamera();

	//m_sound.init(); // 사운드 초기화

	//pd3dDevice->SetTransform( D3DTS_WORLD, &ArnConsts::D3DXMAT_IDENTITY );

	return State::enter( dStartTime );
}

HRESULT IntroState::leave()
{
	if ( m_pLogoDrawRequest != 0 )
		m_pLogo->removeDrawRequest( m_pLogoDrawRequest );
	if ( m_pPrologueDrawRequest != 0 )
		m_pPrologue->removeDrawRequest( m_pPrologueDrawRequest );

	return State::leave();
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

	if ( 6.0f >= m_fFadeTimer )
	{
		ArnMatrix prologueTrans;
		ArnMatrix prologueScale;
		ArnMatrixScaling( &prologueScale, 0.03f, -0.03f, 0.03f );
		ArnMatrixTranslation( 
			&prologueTrans, 
			0.0f, 
			m_prologueInitZPos,
			0.0f );
		m_pPrologueDrawRequest->xform = prologueScale * prologueTrans;
	}	
	if ( 6.0f <= m_fFadeTimer && m_fFadeTimer < 46.0f )
	{	
		ArnMatrix prologueTrans;
		ArnMatrix prologueScale;
		ArnMatrixScaling( &prologueScale, 0.03f, -0.03f, 0.03f );
		ArnMatrixTranslation( 
			&prologueTrans, 
			0.0f, 
			m_prologueInitZPos + (float)m_fFadeTimer * m_velocity,
			0.0f );
		m_pPrologueDrawRequest->xform = prologueScale * prologueTrans;

	}
	else if ( 46.0f <= m_fFadeTimer )
	{
		if ( m_pPrologueDrawRequest != 0 )
			m_pPrologue->removeDrawRequest( m_pPrologueDrawRequest );

		TopStateManager::getSingleton().setNextState( m_nextState );
	}

	if ( 6.0f <= m_fFadeTimer && m_fFadeTimer < 46.0f )
		m_sound.Opening(); // 오프닝 사운드 시작

	return S_OK;
}

HRESULT IntroState::frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{
	/*pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );*/
	

	// Setup view and projection xforms - ?
	pd3dDevice->SetTexture(0, 0);

	//// Apply non-fixed camera
	//pd3dDevice->SetTransform(D3DTS_VIEW, GetG().m_camera.GetViewMatrix());
	//pd3dDevice->SetTransform(D3DTS_PROJECTION, GetG().m_camera.GetProjMatrix());

	//printf("CamAt %f %f %f\n", GetG().m_camera.GetLookAtPt()->x, GetG().m_camera.GetLookAtPt()->y, GetG().m_camera.GetLookAtPt()->z );

	return S_OK;
}

HRESULT IntroState::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN)
	{
		if ( wParam == VK_ESCAPE )
		{
			if ( m_fFadeTimer < 6.0f )
				m_fFadeTimer = 6.0f;
			else
			{
				if ( m_pLogoDrawRequest != 0 )
					m_pLogo->removeDrawRequest( m_pLogoDrawRequest );
				if ( m_pPrologueDrawRequest != 0 )
					m_pPrologue->removeDrawRequest( m_pPrologueDrawRequest );
				TopStateManager::getSingleton().setNextState( m_nextState );
			}
			
			TopStateManager::getSingleton().setNextState( GAME_TOP_STATE_PLAY );
		}
	}

	return S_OK;
}

HRESULT IntroState::release()
{	
	m_pBlack.release();

	/*if ( m_pGalaxy )
		m_pGalaxy->release();
	if ( m_pLogo ) 
		m_pLogo->release();
	if ( m_pLogoDrawRequest )
		m_pLogoDrawRequest->release();
	if ( m_pGalaxyDrawRequest )
		m_pGalaxyDrawRequest->release();*/

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
	ArnVec3 vecEye( 0.0f, -10.0f, -25.0f );
	ArnVec3 vecAt( 0.0f, 0.0f, 0.0f );
	ArnVec3 vecUp( 0.0f, 1.0f, 0.0f );
	GetG().m_camera.setViewParamsWithUp( &vecEye, &vecAt, &vecUp );
}

HRESULT IntroState::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	if ( m_pLogoDrawRequest )
	{
		ArnVec3 logoPos( GetG().m_scrWidth / 2.0f - 124.0f, GetG().m_scrHeight / 2.0f - 64.5f, 0.0f );
		m_pLogoDrawRequest->position = logoPos;
	}
	
	return S_OK;
}

void IntroState::onLostDevice()
{

}

HRESULT IntroState::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	return S_OK;
}
