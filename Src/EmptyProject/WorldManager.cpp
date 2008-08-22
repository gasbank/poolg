#include "EmptyProjectPCH.h"
#include "WorldManager.h"
#include "World.h"
#include "WorldStateManager.h"
#include "ScriptManager.h"
#include "EpLight.h"

IMPLEMENT_SINGLETON( WorldManager )

WorldManager::WorldManager(void)
{
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
		if ( m_curWorld )
			m_curWorld->release();

		m_curWorld = m_nextWorld;
		m_curWorld->init();
		m_nextWorld = 0;

		GetWorldStateManager().setNextStateForced(GAME_WORLD_STATE_FIELD);
		GetEpLight().setFadeDuration( 1.0f );
		GetEpLight().setBrightness( 0.0f );
		GetEpLight().fadeInLightForcedDelayed( 2.0f );
	}
}

void WorldManager::setNextWorld( const char* nextWorldName )
{
	if ( m_worlds.find( nextWorldName ) != m_worlds.end() )
		setNextWorld( m_worlds[ nextWorldName ] );
	else
		throw std::runtime_error( "World name not found" );

	GetEpLight().setFadeDuration( 1.0f );
	GetEpLight().fadeOutLight();
}

void WorldManager::setNextWorld( World* nextWorld )
{
	m_nextWorld = nextWorld;

	GetEpLight().setFadeDuration( 1.0f );
	GetEpLight().fadeOutLight();
}

void WorldManager::detachAllWorlds()
{
	std::map<std::string, World*>::iterator it = m_worlds.begin();
	for ( ; it != m_worlds.end(); ++it )
	{
		EP_SAFE_RELEASE( it->second );
	}
	m_worlds.clear();
}

void WorldManager::release()
{
	detachAllWorlds();
}


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
