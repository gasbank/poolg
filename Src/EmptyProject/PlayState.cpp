#include "EmptyProjectPCH.h"
#include "PlayState.h"
#include "World.h"
#include "ArnSceneGraph.h"
#include "ArnFile.h"


PlayState::PlayState(void)
{
	m_CharactersArnFile			= 0;
	m_CharactersSg				= 0;

	loadArnModels();
}

PlayState::~PlayState(void)
{
}

HRESULT PlayState::enter()
{
	if ( !getCurWorld() )
		throw std::runtime_error( "m_nextWorld should be set before calling enter() method" );

	return S_OK;

}

HRESULT PlayState::leave()
{
	return S_OK;
}

HRESULT PlayState::frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{
	getCurWorld()->frameRender( pd3dDevice, fTime, fElapsedTime );

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
	SAFE_DELETE(m_CharactersArnFile);
	SAFE_DELETE(m_CharactersSg);
	if (m_CharactersArnFile)
		release_arnfile(*m_CharactersArnFile);

	return S_OK;
}

ArnSceneGraph* PlayState::getCharacterSceneGraph()
{
	return m_CharactersSg;
}

void PlayState::loadArnModels()
{
	// Hero model
	m_CharactersArnFile = new ArnFileData;
	load_arnfile( _T("Characters.arn"), *m_CharactersArnFile );
	m_CharactersSg = new ArnSceneGraph( *m_CharactersArnFile );
}