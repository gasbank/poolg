#include "EmptyProjectPCH.h"
#include "WorldManager.h"
#include "World.h"
#include "WorldStateManager.h"
#include "TopStateManager.h"
#include "ScriptManager.h"
#include "EpLight.h"
#include "Hero.h"
#include "Unit.h"
#include "Dialog.h"

IMPLEMENT_SINGLETON( WorldManager )

WorldManager::WorldManager(void)
{
	assert( ScriptManager::getSingletonPtr() );

	m_curWorld = 0;
	m_nextWorld = 0;

}

WorldManager::~WorldManager(void)
{
	release();
}

void WorldManager::addWorld( World* ws )
{
	m_worlds[ ws->getWorldName() ] = ws;
}

void WorldManager::changeToNextWorldIfExist()
{
	if ( m_nextWorld && m_nextWorld != m_curWorld && !GetEpLight().isInFading() )
	{
		Hero* pHero = NULL;
		if ( m_curWorld )
		{
			pHero = m_curWorld->pullOutHero();
			m_curWorld->release();
		}

		m_curWorld = m_nextWorld;
		if ( pHero )
			m_curWorld->addUnit( static_cast<Unit*>(pHero) );
		m_curWorld->init();
		m_nextWorld = 0;

		GetWorldStateManager().setNextStateForced(GAME_WORLD_STATE_FIELD);
	}
}

void WorldManager::setNextWorld( const char* nextWorldName )
{
	if ( m_worlds.find( nextWorldName ) != m_worlds.end() )
		setNextWorld( m_worlds[ nextWorldName ] );
	else
		throw std::runtime_error( "World name not found" );
}

void WorldManager::setNextWorld( World* nextWorld )
{
	m_nextWorld = nextWorld;

}

void WorldManager::detachAllWorlds()
{
	std::map<std::string, World*>::iterator it = m_worlds.begin();
	for ( ; it != m_worlds.end(); ++it )
	{
		EP_SAFE_release( it->second );
	}
	m_worlds.clear();
}

void WorldManager::release()
{
	detachAllWorlds();

	EpSafeReleaseAll( m_globalDialogs );
}

HRESULT WorldManager::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									 void* pUserContext )
{
	if ( m_curWorld )
		m_curWorld->onResetDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );

	return S_OK;
}

void WorldManager::onLostDevice()
{
	if ( m_curWorld )
		m_curWorld->onLostDevice();
}

HRESULT WorldManager::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	if ( m_curWorld )
		m_curWorld->onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc, pUserContext );

	return S_OK;
}
//////////////////////////////////////////////////////////////////////////

World* EpGetCurWorld()
{
	return GetWorldManager().getCurWorld();
} SCRIPT_CALLABLE_PV( EpGetCurWorld )

int EpChangeWorld( const char* worldName )
{
	World* w = GetWorldManager().getWorld( worldName );
	GetWorldManager().setNextWorld( w );
	return 0;
} SCRIPT_CALLABLE_I_PC( EpChangeWorld )

START_SCRIPT_FACTORY( WorldManager )
	CREATE_OBJ_COMMAND( EpGetCurWorld )
	CREATE_OBJ_COMMAND( EpChangeWorld )
END_SCRIPT_FACTORY( WorldManager )
