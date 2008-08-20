#include "EmptyProjectPCH.h"
#include "WorldManager.h"
#include "World.h"
#include "WorldStateManager.h"
#include "ScriptManager.h"

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
	if ( m_nextWorld && m_nextWorld != m_curWorld && !GetG().m_EpLight.isInFading() )
	{
		if ( m_curWorld )
			m_curWorld->release();

		m_curWorld = m_nextWorld;
		m_curWorld->init();
		m_nextWorld = 0;

		GetWorldStateManager().setNextStateForced(GAME_WORLD_STATE_FIELD);
		GetG().m_EpLight.setFadeDuration( 1.0f );
		GetG().m_EpLight.setBrightness( 0.0f );
		GetG().m_EpLight.fadeInLightForcedDelayed( 2.0f );
	}
}

void WorldManager::setNextWorld( const char* nextWorldName )
{
	if ( m_worlds.find( nextWorldName ) != m_worlds.end() )
		setNextWorld( m_worlds[ nextWorldName ] );
	else
		throw std::runtime_error( "World name not found" );

	GetG().m_EpLight.setFadeDuration( 1.0f );
	GetG().m_EpLight.fadeOutLight();
}

void WorldManager::setNextWorld( World* nextWorld )
{
	m_nextWorld = nextWorld;

	GetG().m_EpLight.setFadeDuration( 1.0f );
	GetG().m_EpLight.fadeOutLight();
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
