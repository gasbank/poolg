#include "EmptyProjectPCH.h"
#include "PlayState.h"
#include "World.h"
#include "ArnSceneGraph.h"
#include "SpriteManager.h"
#include "Sprite.h"

PlayState::PlayState(void)
: m_CharactersSg( 0 )
{
	m_CharactersSg = ArnSceneGraph::createFrom("F:/Devel3/Working/models/Characters.xml");

	m_sprite = GetSpriteManager().registerSprite( "PlayUI", "Images/PlayUI/Items.png" );
	m_sprite->setCustomRendered( true );
	m_sprite->registerRect( "GreenButtonBG", 0, 0, 32, 32 );
}

PlayState::~PlayState(void)
{
	delete m_CharactersSg;
	m_CharactersSg = 0;
}

HRESULT PlayState::enter( double dStartTime )
{
	if ( !getCurWorld() )
		throw std::runtime_error( "m_nextWorld should be set before calling enter() method" );

	return State::enter( dStartTime );

}

HRESULT PlayState::leave()
{
	// TODO Current world release if exist

	return S_OK;
}

HRESULT PlayState::frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{
	getCurWorld()->frameRender( pd3dDevice, fTime, fElapsedTime );

	GetSpriteManager().frameRenderSpecificSprite( "PlayUI" );

	return S_OK;
}

HRESULT PlayState::frameMove( double fTime, float fElapsedTime )
{
	getCurWorld()->frameMove( fTime, fElapsedTime );

	//GetWorldManager().changeToNextWorldIfExist()

	m_CharactersSg->getSceneRoot()->update( fTime, fElapsedTime );

	return S_OK;
}

HRESULT PlayState::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	getCurWorld()->handleMessages( hWnd, uMsg, wParam, lParam );
	return S_OK;
}

HRESULT PlayState::release()
{
	return S_OK;
}

ArnSceneGraph* PlayState::getCharacterSceneGraph() const
{
	return m_CharactersSg;
}

void PlayState::loadArnModels()
{
	ArnInitializeRenderableObjectsDx9 (m_CharactersSg);
}

void PlayState::unloadArnModels()
{
	ArnDetachRenderableObjects (m_CharactersSg);
}

HRESULT PlayState::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	loadArnModels();
	return S_OK;
}

void PlayState::onLostDevice()
{
	unloadArnModels();
}

HRESULT PlayState::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	return S_OK;
}

void PlayState::updateHeroItemListThumbnails()
{
	if ( m_sprite )
	{
		m_sprite->clearDrawRequest();
		Character* hero = getCurWorld()->getHeroUnit();
		if ( hero )
		{
			const UINT itemCount = hero->getItemCount();
			UINT i;
			for ( i = 0; i < itemCount; ++i )
			{
				const std::string& itemTnName = hero->getItemThumbnailName( i );
				m_sprite->drawRequest( itemTnName.c_str(), 0, 32 * i, 0, 0, 0xffffffff );
			}
		}
	}
}